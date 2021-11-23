#include "lab_m1/Tema1/tema1.h"

#include <vector>
#include <iostream>

#include "transform.h"
#include "obiect.h"

using namespace std;
using namespace m1;

Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}

void Tema1::Init()
{
    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    // Initialize logicSpave variables
    logicSpace.x = 0;
    logicSpace.y = 0;
    logicSpace.width = 10;
    logicSpace.height = 10;

    // Initialize player position
    playerX = 0;
    playerY = 0;

    // Initialize mouse angle
    mouseAngle = 0;

    //Initialize number of bullets & enemies
    nrBullets = 0;
    nrEnemies = 0;

    // Initialize UI variables
    playerHp = 100;
    score = 0;

    // Initialize time variables
    time = 0;
    deltaTime = 0;
    deltaTimeBullet = 0;

    // Initialize collision checks
    collisionUp = false;
    collisionDown = false;
    collisionLeft = false;
    collisionRight = false;

    // Initialize speed variables
    playerSpeed = 7.0f;
    bulletSpeed = 0.1f;

    Mesh* edge = obiect::CreateRectangle("edge", glm::vec3(0, 0, 0), 1.5f, 3.0f, glm::vec3(0, 0, 0));
    AddMeshToList(edge);
    
    Mesh* obstacle = obiect::CreateRectangle("obstacle", glm::vec3(0, 0, 0), 1.0f, 1.0f, glm::vec3(0.33f, 0.2f, 0.16f));
    AddMeshToList(obstacle);

    Mesh* bullet = obiect::CreateRectangle("bullet", glm::vec3(0, 0, 0), 0.05f, 0.05f, glm::vec3(0, 0, 0));
    AddMeshToList(bullet);

    Mesh* enemy = obiect::CreateRectangle("enemy", glm::vec3(0, 0, 0), 0.5f, 0.5f, glm::vec3(1, 0, 0));
    AddMeshToList(enemy);

    Mesh* body = obiect::CreateTriangle("body", glm::vec3(0, 0, 0), 0.4f, glm::vec3(1, 0.5, 0));
    AddMeshToList(body);

    Mesh* healthBar = obiect::CreateRectangle("healthBar", glm::vec3(0, 0, 0), 4.0f, 0.5f, glm::vec3(1, 0, 0));
    AddMeshToList(healthBar);

    Mesh* healthBarFrame = obiect::CreateUnfilledRectangle("healthBarFrame", glm::vec3(0, 0, 0), 4.0f, 0.5f, glm::vec3(1, 0, 0));
    AddMeshToList(healthBarFrame);

}

glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}

void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}

void Tema1::FrameStart()
{
}

void Tema1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();

    // Sets the screen area where to draw
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0.7, 1, 0.7), true);

    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);

    SpawnEnemies(deltaTimeSeconds);

    DrawScene(visMatrix);

    CollisionDetection();

    if (playerHp <= 0)
    {
        printf("GAME OVER!");
        exit(0);
    }
}

void Tema1::DrawScene(glm::mat3 visMatrix)
{
    float posX = playerX + logicSpace.width / 2;
    float posY = playerY + logicSpace.height / 2;

    //Player
    modelMatrix = visMatrix * transform::Translate(posX , posY);
    modelMatrix *= transform::Translate(0.0f, 0.2f);
    modelMatrix *= transform::Rotate(RADIANS(-mouseAngle + 90));
    modelMatrix *= transform::Translate(-0.0f, -0.2f);
    RenderMesh2D(meshes["body"], shaders["VertexColor"], modelMatrix);

    //UI
    modelMatrix = visMatrix * transform::Translate(posX + logicSpace.width / 2 - 0.25f, posY + logicSpace.height / 2 - 0.75f);
    modelMatrix *= transform::Scale(1.0f * playerHp / 100.0f, 1);
    RenderMesh2D(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform::Translate(posX + logicSpace.width / 2 - 0.25f, posY + logicSpace.height / 2 - 0.75f);
    RenderMesh2D(meshes["healthBarFrame"], shaders["VertexColor"], modelMatrix);

    //Bullets
    for (int i = 0; i < nrBullets; i++)
    {
        if (bullets[i].alive)
        {
            bullets[i].posX += bullets[i].trajectoryX;
            bullets[i].posY += bullets[i].trajectoryY;
            modelMatrix = visMatrix * transform::Translate(bullets[i].posX, bullets[i].posY);
            RenderMesh2D(meshes["bullet"], shaders["VertexColor"], modelMatrix);
        }
    }
    
    //Enemies
    for (int i = 0; i < nrEnemies; i++)
    {
        if (enemies[i].alive)
        {
            enemies[i].trajectoryX = posX - enemies[i].posX;
            enemies[i].trajectoryY = posY - enemies[i].posY;
            float hyp = sqrt(enemies[i].trajectoryX * enemies[i].trajectoryX + enemies[i].trajectoryY * enemies[i].trajectoryY);
            enemies[i].trajectoryX /= hyp;
            enemies[i].trajectoryY /= hyp;
            enemies[i].posX += enemies[i].trajectoryX * enemies[i].enemySpeed;
            enemies[i].posY += enemies[i].trajectoryY * enemies[i].enemySpeed;

            modelMatrix = visMatrix * transform::Translate(enemies[i].posX, enemies[i].posY);
            modelMatrix *= transform::Translate(0.25f, 0.25f);
            modelMatrix *= transform::Rotate(-atan2(enemies[i].trajectoryX, enemies[i].trajectoryY));
            modelMatrix *= transform::Translate(-0.25f, -0.25f);
            RenderMesh2D(meshes["enemy"], shaders["VertexColor"], modelMatrix);
        }
    }

    //Obstacles
    modelMatrix = visMatrix * transform::Translate(-2, 1);
    modelMatrix *= transform::Scale(1, 3);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform::Translate(-2, 1);
    modelMatrix *= transform::Scale(3, 1);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform::Translate(10, 4);
    modelMatrix *= transform::Scale(3, 3);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform::Translate(-4, 7);
    modelMatrix *= transform::Scale(2, 1);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);

    //Left World Edge
    modelMatrix = visMatrix * transform::Translate(-20, -3);
    modelMatrix *= transform::Scale(7, 5);
    RenderMesh2D(meshes["edge"], shaders["VertexColor"], modelMatrix);

    //Right World Edge
    modelMatrix = visMatrix * transform::Translate(20, -3);
    modelMatrix *= transform::Scale(7, 5);
    RenderMesh2D(meshes["edge"], shaders["VertexColor"], modelMatrix);

    //Top World Edge
    modelMatrix = visMatrix * transform::Translate(30, 10);
    modelMatrix *= transform::Rotate(RADIANS(90));
    modelMatrix *= transform::Scale(5, 17);
    RenderMesh2D(meshes["edge"], shaders["VertexColor"], modelMatrix);

    //Bottom World Edge
    modelMatrix = visMatrix * transform::Translate(30, -10);
    modelMatrix *= transform::Rotate(RADIANS(90));
    modelMatrix *= transform::Scale(5, 17);
    RenderMesh2D(meshes["edge"], shaders["VertexColor"], modelMatrix);

}

void Tema1::CollisionDetection()
{
    float X = playerX + logicSpace.width / 2;
    float Y = playerY + logicSpace.height / 2;

    //Player - Obstacles/WorldEdge
    if (Y > 9.6f || (X > -4 && X < -2 && Y > 6.6 && Y < 6.8) || (X > -2 && X < 1 && Y > 0.6 && Y < 0.8) || (X > 10 && X < 13 && Y > 3.6 && Y < 3.8))
        collisionUp = true;
    else
        collisionUp = false;

    if (X < -9.2f || (X  < -0.8 && X > -1 && Y > 1.5 && Y < 4) || (X < -1.8 && X > -2 && Y > 7 && Y < 8) || (X < 1.2 && X > 1 && Y > 1 && Y < 2) || (X < 13.2 && X > 13 && Y < 7 && Y > 4))
        collisionLeft = true;
    else
        collisionLeft = false;

    if (Y < -2.5f || (X > -4 && X < -2 && Y < 8 && Y > 7.8) || (X > -2 && X < -1 && Y < 4 && Y > 3.8) || (X > -1.5 && X < 1 && Y < 2 && Y > 1.8) || (X > 10 && X < 13 && Y < 7.1 && Y > 6.9))
        collisionDown = true;
    else
        collisionDown = false;

    if (X > 19.8f || (X > -4.2 && X < -4 && Y > 7 && Y < 8) || (X > 9.7 && X < 9.9 && Y > 4 && Y < 7) || (X > -2.2 && X < -2 && Y > 1 && Y < 4))
        collisionRight = true;
    else
        collisionRight = false;

    //Enemies
    for (int i = 0; i < nrEnemies; i++)
    {
        if (enemies[i].alive)
        {
            //Player - Enemies
            if (X > enemies[i].posX - 0.25f && X < enemies[i].posX + 0.25f && Y > enemies[i].posY - 0.25f && Y < enemies[i].posY + 0.25f)
            {
                enemies[i].alive = false;
                playerHp -= 10;
            }

            //Bullet - Enemies
            for (int j = 0; j < nrBullets; j++)
            {
                if (bullets[j].alive)
                {
                    if (bullets[j].posX > enemies[i].posX && bullets[j].posX < enemies[i].posX + 0.5f && bullets[j].posY > enemies[i].posY && bullets[j].posY < enemies[i].posY + 0.5f)
                    {
                        bullets[j].alive = false;
                        enemies[i].alive = false;
                        score += 100;
                        printf("Score: %d\n", score);
                    }
                }
            }
        }
    }

    for (int i = 0; i < nrBullets; i++)
    {
        if (bullets[i].alive)
        {
            if (bullets[i].posY > 9.6f ||
                (bullets[i].posX > -4 && bullets[i].posX < -2 && bullets[i].posY > 6.6 && bullets[i].posY < 6.8) ||
                (bullets[i].posX > -2 && bullets[i].posX < 1 && bullets[i].posY > 0.6 && bullets[i].posY < 0.8) ||
                (bullets[i].posX > 10 && bullets[i].posX < 13 && bullets[i].posY > 3.6 && bullets[i].posY < 3.8))
                bullets[i].alive = false;

            if (bullets[i].posX < -9.2f ||
                (bullets[i].posX  < -0.8 && bullets[i].posX > -1 && bullets[i].posY > 1.5 && bullets[i].posY < 4) ||
                (bullets[i].posX < -1.8 && bullets[i].posX > -2 && bullets[i].posY > 7 && bullets[i].posY < 8) ||
                (bullets[i].posX < 1.2 && bullets[i].posX > 1 && bullets[i].posY > 1 && bullets[i].posY < 2) ||
                (bullets[i].posX < 13.2 && bullets[i].posX > 13 && bullets[i].posY < 7 && bullets[i].posY > 4))
                bullets[i].alive = false;

            if (bullets[i].posY < -2.5f ||
                (bullets[i].posX > -4 && bullets[i].posX < -2 && bullets[i].posY < 8 && bullets[i].posY > 7.8) ||
                (bullets[i].posX > -2 && bullets[i].posX < -1 && bullets[i].posY < 4 && bullets[i].posY > 3.8) ||
                (bullets[i].posX > -1.5 && bullets[i].posX < 1 && bullets[i].posY < 2 && bullets[i].posY > 1.8) ||
                (bullets[i].posX > 10 && bullets[i].posX < 13 && bullets[i].posY < 7.1 && bullets[i].posY > 6.9))
                bullets[i].alive = false;

            if (bullets[i].posX > 19.8f ||
                (bullets[i].posX > -4.2 && bullets[i].posX < -4 && bullets[i].posY > 7 && bullets[i].posY < 8) ||
                (bullets[i].posX > 9.7 && bullets[i].posX < 9.9 && bullets[i].posY > 4 && bullets[i].posY < 7) ||
                (bullets[i].posX > -2.2 && bullets[i].posX < -2 && bullets[i].posY > 1 && bullets[i].posY < 4))
                bullets[i].alive = false;
        }
    }
}

void Tema1::SpawnEnemies(float deltaTimeSeconds)
{
    time += deltaTimeSeconds;

    if (time - deltaTime > 1)
    {
        enemies[nrEnemies].posX = (float)(rand() % 20 - rand() % 10);
        enemies[nrEnemies].posY = (float)(rand() % 10);
        enemies[nrEnemies].enemySpeed = (float) (rand() % 10 / 100.0f);
        enemies[nrEnemies].alive = true;
        deltaTime = time;
        nrEnemies++;
    }
}

void Tema1::FrameEnd()
{
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    if (window->KeyHold(GLFW_KEY_W))
    {
        if(!collisionUp)
            playerY += deltaTime * playerSpeed;
        GetSceneCamera()->SetPosition(glm::vec3(playerX * 72, playerY * 72, 50));
        GetSceneCamera()->Update();
    }

    if (window->KeyHold(GLFW_KEY_A))
    {
        if (!collisionLeft)
            playerX -= deltaTime * playerSpeed;
        GetSceneCamera()->SetPosition(glm::vec3(playerX * 72, playerY * 72, 50));
        GetSceneCamera()->Update();
    }

    if (window->KeyHold(GLFW_KEY_S))
    {
        if (!collisionDown)
            playerY -= deltaTime * playerSpeed;
        GetSceneCamera()->SetPosition(glm::vec3(playerX * 72, playerY * 72, 50));
        GetSceneCamera()->Update();
    }

    if (window->KeyHold(GLFW_KEY_D))
    {
        if (!collisionRight)
            playerX += deltaTime * playerSpeed;
        GetSceneCamera()->SetPosition(glm::vec3(playerX * 72, playerY * 72, 50));
        GetSceneCamera()->Update();
    }
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}

void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    mouseAngle = (float) atan2(mouseY - 360, mouseX - 640) * 180.0f / 3.141f;
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (button == GLFW_MOUSE_BUTTON_2 && (time - deltaTimeBullet > 0.5f))
    {
        bullets[nrBullets].posX = playerX + logicSpace.width / 2;
        bullets[nrBullets].posY = playerY + logicSpace.height / 2 + 0.2f;
        bullets[nrBullets].trajectoryX = bulletSpeed * cos(RADIANS(-mouseAngle));
        bullets[nrBullets].trajectoryY = bulletSpeed * sin(RADIANS(-mouseAngle));
        bullets[nrBullets].alive = true;
        deltaTimeBullet = time;
        nrBullets++;
    }
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Add mosue button scroll event
}

void Tema1::OnWindowResize(int width, int height)
{
    // Add window resize event
}