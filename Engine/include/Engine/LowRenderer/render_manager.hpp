#pragma once

#include "singleton.hpp"

#include <vector>
#include <memory>

#include "collider_renderer.hpp"
#include "sprite_renderer.hpp"
#include "model_renderer.hpp"
#include "benchmark.hpp"
#include "sky_box.hpp"
#include "camera.hpp"
#include "light.hpp"

namespace LowRenderer
{
	class RenderManager final : public Singleton<RenderManager>
	{
		friend class Singleton<RenderManager>;

	private:
		RenderManager();
		~RenderManager();

		Core::Engine::Benchmark* bench = nullptr;

		std::vector<std::shared_ptr<ColliderRenderer>> colliders;
		std::vector<std::shared_ptr<ModelRenderer>> models;
		std::vector<std::shared_ptr<SpriteRenderer>> sprites;
		std::vector<std::shared_ptr<Light>> lights;
		std::vector<std::shared_ptr<Camera>> cameras;
		std::vector<std::shared_ptr<SkyBox>> skyBoxes;

		std::shared_ptr<Resources::Texture>			benchTexs[2];
		std::shared_ptr<Resources::Mesh>			benchMesh;
		std::shared_ptr<Resources::ShaderProgram>	benchProgram;

		void drawColliders() const;

		void drawShadows();
		void drawModels();
		void drawSprites();
		void drawBenchmark();

	public:

		static std::shared_ptr<Camera> getCurrentCamera();

		static void draw();

		static void init();

		static void linkComponent(const std::shared_ptr<Light>& compToLink);

		static void linkComponent(const std::shared_ptr<ModelRenderer>& compToLink);

		static void linkComponent(const std::shared_ptr<SpriteRenderer>& compToLink);

		static void linkComponent(const std::shared_ptr<Camera>& compToLink);

		static void linkComponent(const std::shared_ptr<SkyBox>& compToLink);

		static void linkComponent(const std::shared_ptr<ColliderRenderer>& compToLink);

		static void removeComponent(SpriteRenderer* compToRemove);
		static void removeComponent(ModelRenderer* compToRemove);

		template <class C>
		static void clearComponents();

		template<>
		static void clearComponents<Light>()
		{
			instance()->lights.clear();
		}

		template<>
		static void clearComponents<SpriteRenderer>()
		{
			instance()->sprites.clear();
		}

		template<>
		static void clearComponents<ModelRenderer>()
		{
			instance()->models.clear();
		}

		template<>
		static void clearComponents<Camera>()
		{
			instance()->cameras.clear();
		}

		template<>
		static void clearComponents<SkyBox>()
		{
			instance()->skyBoxes.clear();
		}

		template<>
		static void clearComponents<ColliderRenderer>()
		{
			instance()->colliders.clear();
		}
	};
}