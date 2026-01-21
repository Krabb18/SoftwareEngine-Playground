#include "fenster.h"
#include "smallGraphics.h"
#include <iostream>
#include <fstream>
#include <sstream>

#define W 1200
#define H 1000



void clearScreen(fenster &f, uint32_t* buf,int w, int h, uint32_t color)
{
    std::fill(buf, buf + W * H, color);
}

int GetScreenHeight()
{
    return H;
}

int GetScreenWidth() 
{
    return W;
}

void DrawLine(fenster& f,float x0, float y0, float x1, float y1, vec3d col)
{
    float dx = x1 - x0;
    float dy = y1 - y0;

    if(abs(dx) > abs(dy))
    {
        if (x0 > x1)
        {
            float xSave = x0;
            float ySave = y0;
            x0 = x1;
            y0 = y1;
            x1 = xSave;
            y1 = ySave;

        }

        float a = dy / dx;
        float y = y0;

        for (int i = x0; i < x1; i++)
        {
            if (i >= 0 && i < W && y >= 0 && y < H)
            {
                fenster_pixel(&f, (int)i, (int)y) = rgb_value(col.x, col.y, col.z, 255);
                y = y + a;
            }
        }
    }
    else
    {
        if (y0 > y1)
        {
            float xSave = x0;
            float ySave = y0;
            x0 = x1;
            y0 = y1;
            x1 = xSave;
            y1 = ySave;
        }

        float a = dx / dy;
        float x = x0;

        for (int i = y0; i < y1; i++)
        {
            if(i >= 0 && i < H && x >= 0 && x < W)
            {
                fenster_pixel(&f, (int)x, (int)i) = rgb_value(col.x, col.y, col.z, 255);
            }
            
            x = x + a;
        }
    }
}


void DrawTriangle(fenster& f,triangle tri, vec3d col)
{
    DrawLine(f,tri.points[0].x, tri.points[0].y, tri.points[1].x, tri.points[1].y, col);
    DrawLine(f, tri.points[1].x, tri.points[1].y, tri.points[2].x, tri.points[2].y, col);
    DrawLine(f, tri.points[2].x, tri.points[2].y, tri.points[0].x, tri.points[0].y, col);
}


mesh LoadModelObj(string path)
{
    ifstream stream(path);
    string currentLine = "";

    vector<vec3d> vertexPointsContainer;

    mesh myMesh;

    while (getline(stream, currentLine))
    {
        //das hier ist ein vertex
        if (currentLine.find("v") != string::npos && currentLine.find("#") == string::npos)
        {
            std::istringstream ss(currentLine);
            string word;

            vec3d point;

            int counter = 0;
            while (getline(ss, word, ' '))
            {
                if (word != "v")
                {
                    if (counter == 0) { point.x = stof(word); }
                    else if (counter == 1) { point.y = stof(word); }
                    else if (counter == 2) { point.z = stof(word); }
                    counter++;
                }
            }
            vertexPointsContainer.push_back(point);
        }
        else if (currentLine.find("f") != string::npos && currentLine.find("#") == string::npos && currentLine.find("s") == string::npos)
        {
            cout << vertexPointsContainer.size() << endl;


            std::istringstream ss(currentLine);
            string word;

            triangle meshTriangle;
            int counter = 0;
            while (getline(ss, word, ' '))
            {
                if (word != "f")
                {
                    meshTriangle.points[counter] = vertexPointsContainer[stoi(word) - 1];
                    counter++;
                }
            }

            myMesh.triangles.push_back(meshTriangle);
        }
    }

    return myMesh;
}


int main() 
{
    uint32_t *buf = (uint32_t*)malloc(W * H * sizeof(uint32_t));
    struct fenster f = { "hello", W, H, buf };
    fenster_open(&f);

    mesh meshCube;
    meshCube.triangles = {

        // SOUTH
        { 0.0f, 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,},
        { 0.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,},

        // EAST           																			   
        { 1.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,},
        { 1.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,},

        // NORTH           																			   
        { 1.0f, 0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,},
        { 1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,},

        // WEST            																			   
        { 0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,},
        { 0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,},

        // TOP             																			   
        { 0.0f, 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,},
        { 0.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,},

        // BOTTOM          																			  
        { 1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,},
        { 1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,},

    };
    

    mat4x4 projection = Matrix_MakeProjection(90.0f, (float)W / (float)H, 0.1f, 100.0f);
    vec3d vCamera;
    vec3d lookDir;

    //model shit
    mat4x4 matRotX, matRotZ;
    matRotZ = Matrix_MakeRotationZ(20.0f);
    matRotX = Matrix_MakeRotationY(30.0f);

    mat4x4 matTrans;
    matTrans = Matrix_MakeTranslation(0.0f, 0.0f, 6.0f);

    mat4x4 modelMatrix;
    modelMatrix = Matrix_MakeIdentity();
    modelMatrix = Matrix_MultiplyMatrix(matRotZ, matRotX);
    modelMatrix = Matrix_MultiplyMatrix(modelMatrix, matTrans);

    vec3d up = { 0, 1, 0 };
    lookDir = { 0, 0, 1 };
    vec3d target = Vector_Add(vCamera, lookDir);
    mat4x4 matCam = Matrix_PointAt(vCamera, target, up);
    mat4x4 viewMat = Matrix_QuickInverse(matCam);
    float t = 0.0f;

    KrabbTex tex;
    tex.LoadTexture("planks.png", W, H);
    float val = 0.0f;
    while (fenster_loop(&f) == 0) 
    {
        val += 0.01f;
        //model shit
        mat4x4 matRotX, matRotZ;
        matRotZ = Matrix_MakeRotationZ(val);
        matRotX = Matrix_MakeRotationY(val);

        mat4x4 matTrans;
        matTrans = Matrix_MakeTranslation(0.0f, 0.0f, 6.0f);

        mat4x4 modelMatrix;
        modelMatrix = Matrix_MakeIdentity();
        modelMatrix = Matrix_MultiplyMatrix(matRotZ, matRotX);
        modelMatrix = Matrix_MultiplyMatrix(modelMatrix, matTrans);

        //Clear sceen
        clearScreen(f, buf,W, H, rgb_value(100, 155, 255, 255));

        vector<triangle> trianglesToShow;

        //Draw example model
        for(auto &tri : meshCube.triangles)
        {
            triangle triangleProjected, triangleTransformed, triangleView;

            triangleTransformed.points[0] = Matrix_MultiplyVector(modelMatrix, tri.points[0]);
            triangleTransformed.points[1] = Matrix_MultiplyVector(modelMatrix, tri.points[1]);
            triangleTransformed.points[2] = Matrix_MultiplyVector(modelMatrix, tri.points[2]);
            triangleTransformed.texUVs[0] = tri.texUVs[0];
            triangleTransformed.texUVs[1] = tri.texUVs[1];
            triangleTransformed.texUVs[2] = tri.texUVs[2];

            vec3d normal, line1, line2;
            line1 = Vector_Sub(triangleTransformed.points[1], triangleTransformed.points[0]);
            line2 = Vector_Sub(triangleTransformed.points[2], triangleTransformed.points[0]);
            normal = Vector_CrossProduct(line1, line2);
            normal = Vector_Normalise(normal);

            vec3d cameraRay = Vector_Sub(triangleTransformed.points[0], vCamera);

            if (Vector_DotProduct(normal, cameraRay) < 0.0f)
            {
                vec3d lightDirection = { 0.0f, 1.0f, -1.0f };
                lightDirection = Vector_Normalise(lightDirection);
                float dp = max(0.1f, Vector_DotProduct(lightDirection, normal));


                // triangleTransformed.col = c.Attributes;


                triangleView.points[0] = Matrix_MultiplyVector(viewMat, triangleTransformed.points[0]);
                triangleView.points[1] = Matrix_MultiplyVector(viewMat, triangleTransformed.points[1]);
                triangleView.points[2] = Matrix_MultiplyVector(viewMat, triangleTransformed.points[2]);
                triangleView.texUVs[0] = triangleTransformed.texUVs[0];
                triangleView.texUVs[1] = triangleTransformed.texUVs[1];
                triangleView.texUVs[2] = triangleTransformed.texUVs[2];


                int clippedTriangles = 0;
                triangle clipped[2];
                clippedTriangles = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, triangleView, clipped[0], clipped[1]);

                for (int i = 0; i < clippedTriangles; i++)
                {
                    triangleProjected.points[0] = Matrix_MultiplyVector(projection, clipped[i].points[0]);
                    triangleProjected.points[1] = Matrix_MultiplyVector(projection, clipped[i].points[1]);
                    triangleProjected.points[2] = Matrix_MultiplyVector(projection, clipped[i].points[2]);

                    triangleProjected.col = triangleTransformed.col;

                    triangleProjected.texUVs[0] = clipped[i].texUVs[0];
                    triangleProjected.texUVs[1] = clipped[i].texUVs[1];
                    triangleProjected.texUVs[2] = clipped[i].texUVs[2];

                    //scale from -1 and 1 to 0 to 2 ()
                    triangleProjected.points[0] = Vector_Div(triangleProjected.points[0], triangleProjected.points[0].w);
                    triangleProjected.points[1] = Vector_Div(triangleProjected.points[1], triangleProjected.points[1].w);
                    triangleProjected.points[2] = Vector_Div(triangleProjected.points[2], triangleProjected.points[2].w);

                    vec3d vOffsetView = { 1,1,0 };
                    triangleProjected.points[0] = Vector_Add(triangleProjected.points[0], vOffsetView);
                    triangleProjected.points[1] = Vector_Add(triangleProjected.points[1], vOffsetView);
                    triangleProjected.points[2] = Vector_Add(triangleProjected.points[2], vOffsetView);

                    triangleProjected.points[0].x *= 0.5f * (float)GetScreenWidth();
                    triangleProjected.points[0].y *= 0.5f * (float)GetScreenHeight();
                    triangleProjected.points[1].x *= 0.5f * (float)GetScreenWidth();
                    triangleProjected.points[1].y *= 0.5f * (float)GetScreenHeight();
                    triangleProjected.points[2].x *= 0.5f * (float)GetScreenWidth();
                    triangleProjected.points[2].y *= 0.5f * (float)GetScreenHeight();

                    trianglesToShow.push_back(triangleProjected);
                }
            }
        }

        vec3d col;
        col.x = 255;
        col.y = 0;
        col.z = 0;
        for(auto &triangleProjected : trianglesToShow)
        {
            //render triangle
            DrawTriangle(f, triangleProjected, col);
            
            
             TexturedTriangle(triangleProjected.points[0].x, triangleProjected.points[0].y, triangleProjected.texUVs[0].u, triangleProjected.texUVs[0].v, triangleProjected.texUVs[0].w,
                triangleProjected.points[1].x, triangleProjected.points[1].y, triangleProjected.texUVs[1].u, triangleProjected.texUVs[1].v, triangleProjected.texUVs[1].w,
                triangleProjected.points[2].x, triangleProjected.points[2].y, triangleProjected.texUVs[2].u, triangleProjected.texUVs[2].v, triangleProjected.texUVs[2].w, f, tex);
            
           
        }
        trianglesToShow.clear();

        //DrawTriangle(f, tri, col);
        //DrawLine(f,0.0f, 999.0f, 999.0f, 0.0f, col);

    }
    fenster_close(&f);
    return 0;
}