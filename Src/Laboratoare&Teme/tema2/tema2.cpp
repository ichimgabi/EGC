#include "lab_m1/tema2/tema2.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <time.h>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    fov = 70;
    width = 5.0f;
    height = 5.0f;

    bulletSpeed = 1.0f;
    enemySpeed = 1.5f;

    nr_enemies = 0;
    nr_bullets = 0;
    playerAngle = 0;
    firstPerson = false;
    playerHP = 100;
    cooldownHP = false;

    currTime = 0;
    deltaTimeHP = 0;
    deltaTimeBullet = 0;

    //Head, rightHand, lefthand
    {
        glm::vec3 color = glm::vec3(1.0f, 1.0f, 0.9f);
        vector<VertexFormat> vertices =
        {
            VertexFormat(glm::vec3(-1, -1, 1), glm::vec3(0, 0, 1), color),
            VertexFormat(glm::vec3(-1, -1, 3), glm::vec3(0, 1, 0), color),
            VertexFormat(glm::vec3(-1, 1, 1), glm::vec3(1, 0, 0), color),
            VertexFormat(glm::vec3(-1, 1, 3), glm::vec3(0.5, 0.5, 0.5), color),
            VertexFormat(glm::vec3(1, -1, 1), glm::vec3(1, 1, 1), color),
            VertexFormat(glm::vec3(1, -1, 3), glm::vec3(0.2, 0.2, 0.2), color),
            VertexFormat(glm::vec3(1, 1,  1), glm::vec3(0.7, 0.7, 0.7), color),
            VertexFormat(glm::vec3(1, 1,  3), glm::vec3(0, 0, 0), color)

        };

        vector<unsigned int> indices =
        {
            0, 1, 2,
            1, 3, 2,
            2, 3, 7,
            2, 7, 6,
            1, 7, 3,
            1, 5, 7,
            6, 7, 4,
            7, 5, 4,
            0, 4, 1,
            1, 4, 5,
            2, 6, 4,
            0, 2, 4
        };

        CreateMesh("head", vertices, indices);
        CreateMesh("rightHand", vertices, indices);
        CreateMesh("leftHand", vertices, indices);
    }

    //Torso
    {
        glm::vec3 color = glm::vec3(0.1f, 0.1f, 0.1f);
        vector<VertexFormat> vertices =
        {
            VertexFormat(glm::vec3(-1, -1, 1), glm::vec3(0, 0, 1), color),
            VertexFormat(glm::vec3(-1, -1, 3), glm::vec3(0, 1, 0), color),
            VertexFormat(glm::vec3(-1, 1, 1), glm::vec3(1, 0, 0), color),
            VertexFormat(glm::vec3(-1, 1, 3), glm::vec3(0.5, 0.5, 0.5), color),
            VertexFormat(glm::vec3(1, -1, 1), glm::vec3(1, 1, 1), color),
            VertexFormat(glm::vec3(1, -1, 3), glm::vec3(0.2, 0.2, 0.2), color),
            VertexFormat(glm::vec3(1, 1,  1), glm::vec3(0.7, 0.7, 0.7), color),
            VertexFormat(glm::vec3(1, 1,  3), glm::vec3(0, 0, 0), color)

        };

        vector<unsigned int> indices =
        {
            0, 1, 2,
            1, 3, 2,
            2, 3, 7,
            2, 7, 6,
            1, 7, 3,
            1, 5, 7,
            6, 7, 4,
            7, 5, 4,
            0, 4, 1,
            1, 4, 5,
            2, 6, 4,
            0, 2, 4
        };

        CreateMesh("torso", vertices, indices);
    }

    //leftLeg, rightLeg
    {
        glm::vec3 color = glm::vec3(0.5f, 0.0f, 0.0f);
        vector<VertexFormat> vertices =
        {
            VertexFormat(glm::vec3(-1, -1, 1), glm::vec3(0, 0, 1), color),
            VertexFormat(glm::vec3(-1, -1, 3), glm::vec3(0, 1, 0), color),
            VertexFormat(glm::vec3(-1, 1, 1), glm::vec3(1, 0, 0), color),
            VertexFormat(glm::vec3(-1, 1, 3), glm::vec3(0.5, 0.5, 0.5), color),
            VertexFormat(glm::vec3(1, -1, 1), glm::vec3(1, 1, 1), color),
            VertexFormat(glm::vec3(1, -1, 3), glm::vec3(0.2, 0.2, 0.2), color),
            VertexFormat(glm::vec3(1, 1,  1), glm::vec3(0.7, 0.7, 0.7), color),
            VertexFormat(glm::vec3(1, 1,  3), glm::vec3(0, 0, 0), color)

        };

        vector<unsigned int> indices =
        {
            0, 1, 2,
            1, 3, 2,
            2, 3, 7,
            2, 7, 6,
            1, 7, 3,
            1, 5, 7,
            6, 7, 4,
            7, 5, 4,
            0, 4, 1,
            1, 4, 5,
            2, 6, 4,
            0, 2, 4
        };

        CreateMesh("leftLeg", vertices, indices);
        CreateMesh("rightLeg", vertices, indices);
    }

    //leftArm, rightArm
    {
        glm::vec3 color = glm::vec3(0.0f, 0.5f, 0.0f);
        vector<VertexFormat> vertices =
        {
            VertexFormat(glm::vec3(-1, -1, 1), glm::vec3(0, 0, 1), color),
            VertexFormat(glm::vec3(-1, -1, 3), glm::vec3(0, 1, 0), color),
            VertexFormat(glm::vec3(-1, 1, 1), glm::vec3(1, 0, 0), color),
            VertexFormat(glm::vec3(-1, 1, 3), glm::vec3(0.5, 0.5, 0.5), color),
            VertexFormat(glm::vec3(1, -1, 1), glm::vec3(1, 1, 1), color),
            VertexFormat(glm::vec3(1, -1, 3), glm::vec3(0.2, 0.2, 0.2), color),
            VertexFormat(glm::vec3(1, 1,  1), glm::vec3(0.7, 0.7, 0.7), color),
            VertexFormat(glm::vec3(1, 1,  3), glm::vec3(0, 0, 0), color)

        };

        vector<unsigned int> indices =
        {
            0, 1, 2,
            1, 3, 2,
            2, 3, 7,
            2, 7, 6,
            1, 7, 3,
            1, 5, 7,
            6, 7, 4,
            7, 5, 4,
            0, 4, 1,
            1, 4, 5,
            2, 6, 4,
            0, 2, 4
        };

        CreateMesh("leftArm", vertices, indices);
        CreateMesh("rightArm", vertices, indices);
    }

    //Enemy
    {
        glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);
        vector<VertexFormat> vertices =
        {
            VertexFormat(glm::vec3(-1, -1, 1), glm::vec3(0, 0, 1), color),
            VertexFormat(glm::vec3(-1, -1, 3), glm::vec3(0, 1, 0), color),
            VertexFormat(glm::vec3(-1, 1, 1), glm::vec3(1, 0, 0), color),
            VertexFormat(glm::vec3(-1, 1, 3), glm::vec3(0.5, 0.5, 0.5), color),
            VertexFormat(glm::vec3(1, -1, 1), glm::vec3(1, 1, 1), color),
            VertexFormat(glm::vec3(1, -1, 3), glm::vec3(0.2, 0.2, 0.2), color),
            VertexFormat(glm::vec3(1, 1,  1), glm::vec3(0.7, 0.7, 0.7), color),
            VertexFormat(glm::vec3(1, 1,  3), glm::vec3(0, 0, 0), color)

        };

        vector<unsigned int> indices =
        {
            0, 1, 2,
            1, 3, 2,
            2, 3, 7,
            2, 7, 6,
            1, 7, 3,
            1, 5, 7,
            6, 7, 4,
            7, 5, 4,
            0, 4, 1,
            1, 4, 5,
            2, 6, 4,
            0, 2, 4
        };

        CreateMesh("enemy", vertices, indices);
    }

    //Floor
    {
        Mesh* mesh = new Mesh("floor");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    //Bullet
    {
        Mesh* mesh = new Mesh("bullet");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    //Wall
    {
        Mesh* mesh = new Mesh("wall");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    //healthBar
    {
        glm::vec3 color = glm::vec3(0.0f, 1.0f, 0.0f);
        std::vector<VertexFormat> vertices =
        {
            VertexFormat(glm::vec3(0, 0, 0), color),
            VertexFormat(glm::vec3(0, 0, 0) + glm::vec3(5, 0, 0), color),
            VertexFormat(glm::vec3(0, 0, 0) + glm::vec3(5, 1, 0), color),
            VertexFormat(glm::vec3(0, 0, 0) + glm::vec3(0, 1, 0), color)
        };

        std::vector<unsigned int> indices = 
        {
            0, 1, 2,
            0, 2, 3
        };

        CreateMesh("healthBar", vertices, indices);
    }

    {
        glm::vec3 color = glm::vec3(0.0f, 0.3f, 0.0f);
        std::vector<VertexFormat> vertices =
        {
            VertexFormat(glm::vec3(0, 0, 0), color),
            VertexFormat(glm::vec3(0, 0, 0) + glm::vec3(5, 0, 0), color),
            VertexFormat(glm::vec3(0, 0, 0) + glm::vec3(5, 1, 0), color),
            VertexFormat(glm::vec3(0, 0, 0) + glm::vec3(0, 1, 0), color)
        };

        std::vector<unsigned int> indices =
        {
            0, 1, 2,
            0, 2, 3
        };

        CreateMesh("healthBarFrame", vertices, indices);
    }

    //timerBar
    {
        glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
        std::vector<VertexFormat> vertices =
        {
            VertexFormat(glm::vec3(0, 0, 0), color),
            VertexFormat(glm::vec3(0, 0, 0) + glm::vec3(5, 0, 0), color),
            VertexFormat(glm::vec3(0, 0, 0) + glm::vec3(5, 1, 0), color),
            VertexFormat(glm::vec3(0, 0, 0) + glm::vec3(0, 1, 0), color)
        };

        std::vector<unsigned int> indices =
        {
            0, 1, 2,
            0, 2, 3
        };

        CreateMesh("timerBar", vertices, indices);
    }

    {
        glm::vec3 color = glm::vec3(0.3f, 0.3f, 0.3f);
        std::vector<VertexFormat> vertices =
        {
            VertexFormat(glm::vec3(0, 0, 0), color),
            VertexFormat(glm::vec3(0, 0, 0) + glm::vec3(5, 0, 0), color),
            VertexFormat(glm::vec3(0, 0, 0) + glm::vec3(5, 1, 0), color),
            VertexFormat(glm::vec3(0, 0, 0) + glm::vec3(0, 1, 0), color)
        };

        std::vector<unsigned int> indices =
        {
            0, 1, 2,
            0, 2, 3
        };

        CreateMesh("timerBarFrame", vertices, indices);
    }

    GetLabririnth();

    playerX = rand() % 10;
    playerZ = rand() % 10;
    while (lab[(int)playerX][(int)playerZ] != 0)
    {
        playerX = rand() % 10;
        playerZ = rand() % 10;
    }

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (lab[i][j] == 2)
            {
                enemies[nr_enemies].posX = i*5;
                enemies[nr_enemies].posY = 2;
                enemies[nr_enemies].posZ = j*5;
                enemies[nr_enemies].initialPosX = i*5;
                enemies[nr_enemies].initialPosZ = j*5;
                enemies[nr_enemies].alive = true;
                enemies[nr_enemies].forward = true;
                nr_enemies++;
            }
        }
    }

    camera = new implementedT2::CameraT2();
    camera->Set(glm::vec3(playerX * 5, 3, playerZ * 5), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
    playerAngle = sin(camera->GetTargetPosition().x) + sin(camera->GetTargetPosition().y);
    
    projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);

}

void Tema2::GetLabririnth()
{
    srand(time(NULL));
    string input = "labirinturi/" + to_string(rand() % 5 + 1) + ".txt";

    ifstream file_in;
    file_in.open(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "labirinturi", to_string(rand() % 5 + 1) + ".txt"));

    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            file_in >> lab[i][j];

    file_in.close();
    cout << PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "labirinturi", to_string(rand() % 5 + 1) + ".txt") << endl;
}

void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    window->DisablePointer();

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::CreateMesh(const char* name, const vector<VertexFormat>& vertices, const vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO = 0;
    // Create the VBO and bind it
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    unsigned int IBO = 0;
    // Create the IBO and bind it
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));

    glBindVertexArray(0);

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
}

void Tema2::Update(float deltaTimeSeconds)
{
    CollisionDetection();

    currTime += deltaTimeSeconds;

    if (cooldownHP == true && currTime - deltaTimeHP > 2)
        cooldownHP = false;

    //Render 2D
    {
        projectionMatrix = glm::ortho(-width, width, -height, height, 0.01f, 200.0f);

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D_t2::Translate(camera->GetTargetPosition().x, camera->GetTargetPosition().y, camera->GetTargetPosition().z);
        modelMatrix *= transform3D_t2::Scale(1.0f * playerHP / 100.0f, 1, 1.0f * playerHP / 100.0f);
        modelMatrix *= transform3D_t2::RotateOY(playerAngle);
        RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D_t2::Translate(camera->GetTargetPosition().x, camera->GetTargetPosition().y, camera->GetTargetPosition().z);
        modelMatrix *= transform3D_t2::RotateOY(playerAngle);
        RenderMesh(meshes["healthBarFrame"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D_t2::Translate(camera->GetTargetPosition().x, camera->GetTargetPosition().y - 1.5f, camera->GetTargetPosition().z);
        modelMatrix *= transform3D_t2::Scale((120 - currTime) / 120, 1, (120 - currTime) / 120);
        modelMatrix *= transform3D_t2::RotateOY(playerAngle);
        RenderMesh(meshes["timerBar"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D_t2::Translate(camera->GetTargetPosition().x, camera->GetTargetPosition().y - 1.5f, camera->GetTargetPosition().z);
        modelMatrix *= transform3D_t2::RotateOY(playerAngle);
        RenderMesh(meshes["timerBarFrame"], shaders["VertexColor"], modelMatrix);
    }

    //HPcheck
    if (playerHP <= 0)
    {
        cout << "You got caught! Game Over!" << endl;
        window->Close();
    }

    //TimerCheck
    if (currTime >= 120)
    {
        cout << "You didn't make it to the end in time! Game Over!" << endl;
        window->Close();
    }

    //Render 3D
    projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);

    //Player
    if (firstPerson == false)
    {
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D_t2::Translate(0.0f + camera->GetTargetPosition().x, 2.3f, camera->GetTargetPosition().z);
        modelMatrix *= transform3D_t2::RotateOY(playerAngle);
        modelMatrix *= transform3D_t2::Scale(0.2f, 0.2f, 0.2f);
        RenderMesh(meshes["head"], shaders["VertexNormal"], modelMatrix);

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D_t2::Translate(camera->GetTargetPosition().x, 0.8f, camera->GetTargetPosition().z);
        modelMatrix *= transform3D_t2::RotateOY(playerAngle);
        modelMatrix *= transform3D_t2::Translate(0.6f, 0.0f, 0.0f);
        modelMatrix *= transform3D_t2::Scale(0.2f, 0.2f, 0.3f);
        RenderMesh(meshes["rightHand"], shaders["VertexNormal"], modelMatrix);

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D_t2::Translate(camera->GetTargetPosition().x, 0.8f, camera->GetTargetPosition().z);
        modelMatrix *= transform3D_t2::RotateOY(playerAngle);
        modelMatrix *= transform3D_t2::Translate(-0.6f, 0.0f, 0.0f);
        modelMatrix *= transform3D_t2::Scale(0.2f, 0.2f, 0.3f);
        RenderMesh(meshes["leftHand"], shaders["VertexNormal"], modelMatrix);

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D_t2::Translate(camera->GetTargetPosition().x, 1.4f, camera->GetTargetPosition().z);
        modelMatrix *= transform3D_t2::RotateOY(playerAngle);
        modelMatrix *= transform3D_t2::Translate(0.6f, 0.0f, 0.0f);
        modelMatrix *= transform3D_t2::Scale(0.2f, 0.4f, 0.3f);
        RenderMesh(meshes["rightArm"], shaders["VertexNormal"], modelMatrix);

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D_t2::Translate(camera->GetTargetPosition().x, 1.4f, camera->GetTargetPosition().z);
        modelMatrix *= transform3D_t2::RotateOY(playerAngle);
        modelMatrix *= transform3D_t2::Translate(-0.6f, 0.0f, 0.0f);
        modelMatrix *= transform3D_t2::Scale(0.2f, 0.4f, 0.3f);
        RenderMesh(meshes["leftArm"], shaders["VertexNormal"], modelMatrix);

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D_t2::Translate(camera->GetTargetPosition().x, 0.3f, camera->GetTargetPosition().z);
        modelMatrix *= transform3D_t2::RotateOY(playerAngle);
        modelMatrix *= transform3D_t2::Translate(0.2f, 0.0f, 0.0f);
        modelMatrix *= transform3D_t2::Scale(0.18f, 0.4f, 0.3f);
        RenderMesh(meshes["rightLeg"], shaders["VertexNormal"], modelMatrix);

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D_t2::Translate(camera->GetTargetPosition().x, 0.3f, camera->GetTargetPosition().z);
        modelMatrix *= transform3D_t2::RotateOY(playerAngle);
        modelMatrix *= transform3D_t2::Translate(-0.2f, 0.0f, 0.0f);
        modelMatrix *= transform3D_t2::Scale(0.18f, 0.4f, 0.3f);
        RenderMesh(meshes["leftLeg"], shaders["VertexNormal"], modelMatrix);

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D_t2::Translate(camera->GetTargetPosition().x, 1.4f, camera->GetTargetPosition().z);
        modelMatrix *= transform3D_t2::RotateOY(playerAngle);
        modelMatrix *= transform3D_t2::Scale(0.4f, 0.7f, 0.3f);
        RenderMesh(meshes["torso"], shaders["VertexNormal"], modelMatrix);
    }

    //Enemies
    for (int i = 0; i < nr_enemies; i++)
    {
        if (enemies[i].alive)
        {
            if (enemies[i].forward == true)
            {
                if (enemies[i].posX <= enemies[i].initialPosX + 1)
                {
                    enemies[i].posX += 0.05f;
                    enemies[i].posZ += 0.05f;
                }
                else
                    enemies[i].forward = false;
            }
            else
            {
                if (enemies[i].posX >= enemies[i].initialPosX - 1)
                {
                    enemies[i].posX -= 0.05f;
                    enemies[i].posZ -= 0.05f;
                }
                else
                    enemies[i].forward = true;
            }

            modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D_t2::Translate(enemies[i].posX, enemies[i].posY, enemies[i].posZ);
            modelMatrix *= transform3D_t2::Scale(0.5f, 2, 0.5f);
            RenderMesh(meshes["enemy"], shaders["VertexNormal"], modelMatrix);
        }
    }

    //Labirinth
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (lab[i][j] == 1)
            {
                modelMatrix = glm::mat4(1);
                modelMatrix *= transform3D_t2::Translate(5*i, 1.5f, 5*j);
                modelMatrix *= transform3D_t2::Scale(5, 5, 5);
                RenderMesh(meshes["wall"], shaders["VertexNormal"], modelMatrix);
            }
        }
    }

    //Floor
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D_t2::Scale(3, 1, 3);
    RenderMesh(meshes["floor"], shaders["Simple"], modelMatrix);

    //Bullets
    for (int i = 0; i < nr_bullets; i++)
    {
        if (bullets[i].alive)
        {
            bullets[i].posX += bullets[i].trajectoryX;
            bullets[i].posZ += bullets[i].trajectoryZ;
            modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D_t2::Translate(bullets[i].posX, bullets[i].posY, bullets[i].posZ);
            modelMatrix *= transform3D_t2::Scale(0.5f, 0.5f, 0.5f);
            RenderMesh(meshes["bullet"], shaders["VertexNormal"], modelMatrix);
        }
    }

    //EndGame
    if (camera->GetTargetPosition().x > 50 || camera->GetTargetPosition().x < 0 ||
        camera->GetTargetPosition().z > 50 || camera->GetTargetPosition().z < 0)
    {
        window->Close();
        cout << "Congratulations! You reached the end!" << endl;
    }
}


void Tema2::FrameEnd()
{
}

void Tema2::CollisionDetection()
{
    //Player - Wall


    //Player - Enemy
    for (int j = 0; j < nr_enemies; j++)
    {
        if (cooldownHP == false)
        {
            if (camera->GetTargetPosition().x - 1 <= enemies[j].posX && camera->GetTargetPosition().x + 1 >= enemies[j].posX - 1 &&
                (camera->GetTargetPosition().z + 1 <= enemies[j].posZ && camera->GetTargetPosition().z + 2 >= enemies[j].posZ - 1))
            {
                playerHP -= 25;
                cooldownHP = true;
                deltaTimeHP = currTime;
                cout << playerHP << endl;
            }
        }
    }

    //Bullet - Enemy
    for (int i = 0; i < nr_bullets; i++)
    {
        for (int j = 0; j < nr_enemies; j++)
        {
            if (bullets[i].posX >= enemies[j].posX - 1 && bullets[i].posX <= enemies[j].posX &&
                (bullets[i].posZ >= enemies[j].posZ - 1 && bullets[i].posZ <= enemies[j].posZ))
            {
                bullets[i].alive = false;
                enemies[j].alive = false;
            }
        }
    }
}

void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    float playerSpeed = 3.5f;
    float cameraSpeed = 3.5f;

    //Miscare player & camera
    if (window->KeyHold(GLFW_KEY_W)) {
        playerZ += deltaTime * playerSpeed;
        camera->MoveForward(deltaTime * cameraSpeed);
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        playerX += deltaTime * playerSpeed;
        camera->TranslateRight(-deltaTime * cameraSpeed);
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        playerZ += -deltaTime * playerSpeed;
        camera->MoveForward(-deltaTime * cameraSpeed);
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        playerX += -deltaTime * playerSpeed;
        camera->TranslateRight(deltaTime * cameraSpeed);
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        if (key == GLFW_KEY_SPACE)
        {
            bullets[nr_bullets].posX = camera->GetTargetPosition().x;
            bullets[nr_bullets].posY = camera->GetTargetPosition().y;
            bullets[nr_bullets].posZ = camera->GetTargetPosition().z;
            bullets[nr_bullets].trajectoryX = bulletSpeed * cos(-playerAngle - 3.14/2);
            bullets[nr_bullets].trajectoryZ = bulletSpeed * sin(-playerAngle - 3.14/2);
            bullets[nr_bullets].alive = true;
            nr_bullets++;
        }
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    float sensivityOX = 0.003f;
    float sensivityOY = 0.003f;

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        firstPerson = true;
        camera->RotateFirstPerson_OX(RADIANS(-sensivityOX * deltaY));
        camera->RotateFirstPerson_OY(-sensivityOY * deltaX);
        playerAngle += -sensivityOY * deltaX;
    }
    else
    {
        firstPerson = false;
        camera->RotateThirdPerson_OX(-sensivityOX * deltaY);
        camera->RotateThirdPerson_OY(-sensivityOY * deltaX);
        playerAngle += -sensivityOY * deltaX;
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
