#include <cg/CGRenderer.h>

#include <iostream>
#include <AntTweakBar.h>



namespace cgbv
{
    CGRenderer::CGRenderer(GLFWwindow *window) : Renderer(window)
    {

    }


    CGRenderer::~CGRenderer()
    {

    }


    void CGRenderer::destroy()
    {
        glDeleteVertexArrays(1, &cone.vao);
        glDeleteBuffers(1, &cone.vbo);
		glDeleteSamplers(1, &sampler);
    }


    void CGRenderer::resize(int width, int height)
    {
        window_width = width;
        window_height = height;

        glViewport(0, 0, width, height);

        projection = glm::perspective(float(M_PI) / 5.f, float(window_width) / float(window_height), .1f, 200.f);

        TwWindowSize(width, height > 0 ? height : 1);
    }


    void CGRenderer::input(int key, int scancode, int action, int modifiers)
    {
        TwEventCharGLFW(key, action);
        TwEventKeyGLFW(key, action);

        if(action == GLFW_PRESS)
        {
            switch (key)
            {
                case GLFW_KEY_W:
                    camera.moveForward(.1f);
                    break;
                case GLFW_KEY_S:
                    camera.moveForward(-.1f);
                    break;
                case GLFW_KEY_A:
                    camera.moveRight(.1f);
                    break;
                case GLFW_KEY_D:
                    camera.moveRight(-.1f);
                    break;
                default:
                    break;
            }
        }
    }


    bool CGRenderer::setup()
    {
        glfwGetFramebufferSize(window, &window_width, &window_height);


        if(!gladLoadGL())
            return false;


        // GL States
        glClearColor(0.4f, 0.4f, 0.4f, 1.f);

        glEnable(GL_ALPHA_TEST);
        glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



        projection = glm::perspective(float(M_PI) / 5.f, float(window_width) / float(window_height), .1f, 200.f);
		camera.setTarget(glm::vec3(0.f, 0.f, 0.f));
		camera.moveTo(0.f, -.5f, 5.f);


        // Shader
        shader = std::make_unique<cgbv::shader::GLSLShaderprogram>("../shader/cg/VertexShader.glsl", "../shader/cg/FragmentShader.glsl");
        locs.vertex = shader->getAttribLocation("vertex");
        locs.uv = shader->getAttribLocation("uvs");
        locs.modelViewProjection = shader->getUniformLocation("matrices.mvp");
        locs.normalmatrix = shader->getUniformLocation("matrices.normal");
        locs.modelview = shader->getUniformLocation("matrices.mv");
        locs.lightPos = shader->getUniformLocation("light.lightPos");
		locs.texture = shader->getUniformLocation("textures.tex");
		locs.animationUVs = shader->getUniformLocation("animStage");
        locs.subFragment = shader->getSubroutineIndex(GL_FRAGMENT_SHADER, "textured");
        locs.subVertex = shader->getSubroutineIndex(GL_VERTEX_SHADER, "verts_and_normals");




        // Geometrie
        std::vector<float> data = 
		{
			-1.f, -1.f, 0.f,
			0.f, 0.f,
			1.f, -1.f, 0.f,
			1.f * .04f, 0.f,
			-1.f, 1.f, 0.f,
			0.f, 1.f,

			-1.f, 1.f, 0.f,
			0.f, 1.f,
			1.f, -1.f, 0.f,
			1.f * .04f, 0.f,
			1.f, 1.f, 0.f,
			1.f * .04f, 1.f
		};
		cone.vertsToDraw = 6;

        glGenVertexArrays(1, &cone.vao);
        glBindVertexArray(cone.vao);

        glGenBuffers(1, &cone.vbo);
        glBindBuffer(GL_ARRAY_BUFFER, cone.vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(locs.vertex);
        glVertexAttribPointer(locs.vertex, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
        glEnableVertexAttribArray(locs.uv);
        glVertexAttribPointer(locs.uv, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*) size_t(3 * sizeof(float)));



		// Texturen
		glGenSamplers(1, &sampler);
		glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glSamplerParameterf(sampler, GL_TEXTURE_MAX_ANISOTROPY, 16.f);

		texture = std::make_unique<cgbv::textures::Texture2D>();
		texture->Generate("../textures/cg/magic_003.png", true);



        // GUI
        TwInit(TW_OPENGL_CORE, nullptr);
        TwWindowSize(1280, 720);
        TwBar *tweakbar = TwNewBar("TweakBar");
        TwDefine(" TweakBar size='300 400'");

        TwAddVarRW(tweakbar, "Global Rotation", TW_TYPE_QUAT4F, &parameter.globalRotation, "showval=false opened=true");
        TwAddVarRW(tweakbar, "Lichtrichtung", TW_TYPE_DIR3F, &parameter.lightPos, "group=Light axisx=-x axisy=-y axisz=-z opened=true");

        return true;

		last = std::chrono::high_resolution_clock::now();
    }


    void CGRenderer::render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //camera.setTarget(glm::vec3(0.f, 0.f, 0.f));
        //camera.moveTo(0.f, -.5f, 5.f);
        glm::mat4 view = camera.getViewMatrix();


        model = glm::mat4_cast(parameter.globalRotation);


        shader->use();
        normal = glm::transpose(glm::inverse(view * model));
        glUniformMatrix4fv(locs.modelViewProjection, 1, GL_FALSE, glm::value_ptr(projection * view * model));
        glUniformMatrix4fv(locs.modelview, 1, GL_FALSE, glm::value_ptr(view * model));
        glUniformMatrix3fv(locs.normalmatrix, 1, GL_FALSE, glm::value_ptr(normal));


        glUniform3fv(locs.lightPos, 1, glm::value_ptr(parameter.lightPos));

		glActiveTexture(GL_TEXTURE0);
		glBindSampler(0, sampler);
		glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
		glUniform1i(locs.texture, 0);


        glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &locs.subVertex);
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &locs.subFragment);

		glUniform1f(locs.animationUVs, animStage);

        glBindVertexArray(cone.vao);
        glDrawArrays(GL_TRIANGLES, 0, cone.vertsToDraw);

		glBindSampler(0, 0);
		glBindTexture(GL_TEXTURE_2D, 0);


        TwDraw();
    }


    void CGRenderer::update()
    {
		auto now = std::chrono::high_resolution_clock::now();

		std::chrono::duration<float, std::milli> delta = now - last;

		animStage += (delta.count() * 0.02f);

		if (animStage >= 25.f)
			animStage = 0.f;

		last = now;
    }
}