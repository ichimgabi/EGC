#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema2/camera3D.h"
#include "lab_m1/tema2/transform3D_t2.h"

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
        void GetLabririnth();
        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
        void CollisionDetection();

    protected:
        implementedT2::CameraT2* camera;
        glm::mat4 projectionMatrix;
        glm::mat4 modelMatrix;
        float playerX, playerY, playerZ;
        float playerAngle;
        int playerHP;
        bool cooldownHP;
        float deltaTimeHP, deltaTimeBullet, currTime;

        float fov, width, height;
        int lab[10][10];
        int nr_enemies, nr_bullets;
        float enemySpeed, bulletSpeed;

        bool firstPerson;

        struct enemies {
            float posX, posY, posZ;
            float initialPosX, initialPosZ;
            bool alive, forward;
        } enemies[100];

        struct bullets {
            float posX, posY, posZ;
            float trajectoryX, trajectoryY, trajectoryZ;
            bool alive;
        } bullets[1000];
    };
}   // namespace m1
