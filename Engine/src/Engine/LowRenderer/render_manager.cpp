#include "render_manager.hpp"

#include <algorithm>

#include "time.hpp"
#include "debug.hpp"
#include "benchmark.hpp"
#include "resources_manager.hpp"

#include "shader.hpp"
#include "utils.hpp"
#include "shadow.hpp"


namespace LowRenderer
{
	RenderManager::RenderManager()
	{
		Core::Debug::Log::info("Creating the Render Manager");
	}

	RenderManager::~RenderManager()
	{
		models.clear();
		lights.clear();
		sprites.clear();

		Core::Debug::Log::info("Destroying the Render Manager");
	}

	void RenderManager::init()
	{
		RenderManager* RM = instance();

		RM->benchMesh = Resources::ResourcesManager::getMeshByName("plane");
		RM->benchProgram = Resources::ResourcesManager::loadShaderProgram("spriteShader");

		RM->benchTexs[0] = Resources::ResourcesManager::loadTexture("resources/UI/benchText.png");
		RM->benchTexs[1] = Resources::ResourcesManager::loadTexture("resources/UI/benchWheel.png");

		RM->bench = Core::Engine::Benchmark::getInstance();
	}

	void RenderManager::drawShadows()
	{
		std::shared_ptr<Resources::ShaderProgram> program;

		glCullFace(GL_FRONT);

		// Number of lights to render (8 max)
		int lightCount = std::min((int)lights.size(), 8);

		for (int i = 0; i < lightCount; i++)
			lights[i]->compute();

		for (auto& light : lights)
		{
			if (!light->isActive() || light->shadow == nullptr)
				continue;

			program = light->shadow->program;

			program->bind();

			light->shadow->sendToShader(light);

			light->shadow->bindAndSetViewport();

			glClear(GL_DEPTH_BUFFER_BIT);

			for (auto& model : models)
				model->simpleDraw(program);

			light->shadow->unbindAndResetViewport();
		}
	}

	void RenderManager::drawModels()
	{
		std::shared_ptr<Resources::ShaderProgram> program;

		if (skyBoxes.size() > 0)
		{
			if (skyBoxes.back()->isActive())
				skyBoxes.back()->draw();
		}

		glEnable(GL_FRAMEBUFFER_SRGB);
		glCullFace(GL_BACK);

		// Number of lights to render (8 max)
		int lightCount = std::min((int)lights.size(), 8);

		// Draw renderers
		for (std::shared_ptr<ModelRenderer>& model : models)
		{
			if (!model->isActive())
				continue;

			// If the ShaderProgram has changed, bind it and send shared informations
			if (program != model->getProgram())
			{
				program = model->getProgram();

				program->bind();

				getCurrentCamera()->sendViewProjToProgram(program);

				for (int i = 0; i < lightCount; i++)
					lights[i]->sendToProgram(program, i);
			}

			model->draw();
		}

		program->unbind();

		drawColliders();
	}

	void RenderManager::drawSprites()
	{
		std::shared_ptr<Resources::ShaderProgram> program;

		glClear(GL_DEPTH_BUFFER_BIT);

		// Draw renderers
		for (std::shared_ptr<SpriteRenderer>& sprite : sprites)
		{
			if (!sprite->isActive())
				continue;

			// If the ShaderProgram has changed, bind it and send shared informations
			if (program != sprite->getProgram())
			{
				program = sprite->getProgram();

				program->bind();

				getCurrentCamera()->sendViewOrthoToProgram(program);
			}

			sprite->draw();
		}

		program->unbind();

		glDisable(GL_FRAMEBUFFER_SRGB);
	}

	void drawSpriteBench(std::shared_ptr<Resources::ShaderProgram>& program, std::shared_ptr<Resources::Mesh>& mesh, std::shared_ptr<Resources::Texture>& texture, const Core::Maths::mat4& model)
	{
		Core::Maths::vec4 color = { 1.f, 1.f, 1.f, 1.f };

		program->setUniform("model", model.e, 1, 1);
		program->setUniform("color", color.e);

		program->setUniform("tilling", Core::Maths::vec2(1.f, 1.f).e);

		int i = 0;
		program->setUniform("diffuseTex", &i);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->getID());

		mesh->draw();
	}

	void RenderManager::drawBenchmark()
	{
		glEnable(GL_FRAMEBUFFER_SRGB);
		//glCullFace(GL_BACK);

		glClear(GL_DEPTH_BUFFER_BIT);

		static int index = 0;
		static float angle = 0.f;

		angle += 250.f * Core::TimeManager::getDeltaTime();

		angle = Utils::clampLoop(angle, 0.f, 360.f);

		Core::Maths::mat4 modelText = Core::Maths::translate(Core::Maths::vec3(0.f, 0.f, 0.f)) * 
								  Core::Maths::scale(Core::Maths::vec3(2.f, 2.f, 2.f));

		Core::Maths::mat4 modelWheel = Core::Maths::translate(Core::Maths::vec3(1.2f, 0.f, 0.1f)) * Core::Maths::rotateZ(-angle * Core::Maths::DEG2RAD) *
			Core::Maths::scale(Core::Maths::vec3(0.5f, 0.5f, 0.5f));

		benchProgram->bind();

		getCurrentCamera()->sendViewOrthoToProgram(benchProgram);

		drawSpriteBench(benchProgram, benchMesh, benchTexs[0], modelText);
		drawSpriteBench(benchProgram, benchMesh, benchTexs[1], modelWheel);

		benchProgram->unbind();

		glDisable(GL_FRAMEBUFFER_SRGB);
	}

	void RenderManager::draw()
	{
		RenderManager* RM = instance();

		if (RM->bench->isActive())
		{
			RM->drawBenchmark();
			return;
		}

		RM->drawShadows();
		RM->drawModels();
		RM->drawSprites();
	}

	void RenderManager::drawColliders() const
	{
		if (colliders.size() == 0)
			return;

		glDisable(GL_DEPTH_TEST);

		std::shared_ptr<Resources::ShaderProgram> program = colliders[0]->getProgram();

		program->bind();

		getCurrentCamera()->sendProjToProgram(program);

		for (const std::shared_ptr<ColliderRenderer>& rendererCollider : colliders)
		{
			if (rendererCollider->canBeDraw())
				rendererCollider->draw();
		}

		program->unbind();

		glEnable(GL_DEPTH_TEST);
	}

	void RenderManager::linkComponent(const std::shared_ptr<Light>& compToLink)
	{
		// Insert light to render
		instance()->lights.push_back(compToLink);
	}

	void RenderManager::linkComponent(const std::shared_ptr<ModelRenderer>& compToLink)
	{
		// Insert model to renderer
		instance()->models.push_back(compToLink);
	}

	void RenderManager::linkComponent(const std::shared_ptr<SpriteRenderer>& compToLink)
	{
		// Insert sprite to renderer
		instance()->sprites.push_back(compToLink);
	}

	void RenderManager::linkComponent(const std::shared_ptr<Camera>& compToLink)
	{
		// Insert camera to render
		instance()->cameras.push_back(compToLink);
	}

	void RenderManager::linkComponent(const std::shared_ptr<SkyBox>& compToLink)
	{
		// Insert camera to render
		instance()->skyBoxes.push_back(compToLink);
	}

	void RenderManager::linkComponent(const std::shared_ptr<ColliderRenderer>& compToLink)
	{
		// Insert camera to render
		instance()->colliders.push_back(compToLink);
	}

	void RenderManager::removeComponent(SpriteRenderer* compToRemove)
	{
		RenderManager* RM = instance();

		for (auto it = RM->sprites.begin(); it != RM->sprites.end(); it++)
		{
			if (it->get() == compToRemove)
			{
				RM->sprites.erase(it);
				break;
			}
		}
	}

	void RenderManager::removeComponent(ModelRenderer* compToRemove)
	{
		RenderManager* RM = instance();

		for (auto it = RM->models.begin(); it != RM->models.end(); it++)
		{
			if (it->get() == compToRemove)
			{
				RM->models.erase(it);
				break;
			}
		}
	}

	std::shared_ptr<Camera> RenderManager::getCurrentCamera()
	{
		RenderManager* RM = instance();

		if (RM->cameras.size() > 0)
			return RM->cameras.back();

		return nullptr;
	}
}