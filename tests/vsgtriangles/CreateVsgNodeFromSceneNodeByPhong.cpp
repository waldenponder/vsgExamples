/// <summary>
/// 从场景节点创建VSG节点，使用vsg phone着色器
/// </summary>
/// <param name="node"></param>
/// <returns></returns>
vsg::ref_ptr<vsg::Group> BimMainWindow::CreateVsgNodeFromSceneNodeByPhong(const std::shared_ptr<Bim::Scene::Node>& node)
{
	auto scenegraph = vsg::Group::create();

	auto sharedObjects = vsg::SharedObjects::create();
    //1.创建一个Phone模型的渲染管线；
	auto shaderSet = vsg::createPhongShaderSet(m_vsgOptions);
	auto graphicsPipelineConfig = vsg::GraphicsPipelineConfigurator::create(shaderSet);
    //2.填充默认材质；
	auto mat = vsg::PhongMaterialValue::create();
	mat->value().diffuse.set(1.0f, 1.0f, 1.0f, 1.0f);
	mat->value().specular.set(1.0f, 1.0f, 1.0f, 1.0f); // red specular highlight
	graphicsPipelineConfig->assignDescriptor("material", mat);
	
	
	if (node->model)
	{
		for (auto& geometry : node->model->geometries)
		{
			if (geometry->mesh)
			{
				auto& mesh = geometry->mesh;
				//顶点
				static_assert(sizeof(vsg::vec3) == 3 * sizeof(float));
				size_t numVertices = mesh->vertices.size() / 3;
				auto vertices = vsg::vec3Array::create(numVertices);
				std::memcpy(vertices->data(), mesh->vertices.data(), mesh->vertices.size() * sizeof(float));
				//法向量
				vsg::ref_ptr<vsg::vec3Array> normals = nullptr;
				if (!mesh->normals.empty()) {
					size_t numNormals = mesh->normals.size() / 3;
					normals = vsg::vec3Array::create(numNormals);
					std::memcpy(normals->data(), mesh->normals.data(), mesh->normals.size() * sizeof(float));
				}
				//顶点颜色
				vsg::ref_ptr<vsg::vec4Array> colors = nullptr;
				if (!mesh->colors.empty()) {
					size_t numColors = mesh->colors.size() / 4;
					colors = vsg::vec4Array::create(numColors);
					std::memcpy(colors->data(), mesh->colors.data(), mesh->colors.size() * sizeof(float));
				}
                //3.填充顶点，法线，颜色等数据；
				vsg::DataList vertexArrays;
				graphicsPipelineConfig->assignArray(vertexArrays, "vsg_Vertex", VK_VERTEX_INPUT_RATE_VERTEX, vertices);
				if (normals) {
					graphicsPipelineConfig->assignArray(vertexArrays, "vsg_Normal", VK_VERTEX_INPUT_RATE_VERTEX, normals);
				}
				//graphicsPipelineConfig->assignArray(vertexArrays, "vsg_TexCoord0", VK_VERTEX_INPUT_RATE_VERTEX, texcoords);
				graphicsPipelineConfig->assignArray(vertexArrays, "vsg_Color", VK_VERTEX_INPUT_RATE_INSTANCE, colors);

				//索引
				auto indices = vsg::uintArray::create(mesh->indices.size());
				std::copy(mesh->indices.begin(), mesh->indices.end(), indices->begin());

				// 4.绘制命令
				auto drawCommands = vsg::Commands::create();
				drawCommands->addChild(vsg::BindVertexBuffers::create(graphicsPipelineConfig->baseAttributeBinding, vertexArrays));
				drawCommands->addChild(vsg::BindIndexBuffer::create(indices));
				drawCommands->addChild(vsg::DrawIndexed::create(indices->size(), 1, 0, 0, 0));

				if (sharedObjects) sharedObjects->share(vertexArrays);
				if (sharedObjects) sharedObjects->share(indices);

				if (sharedObjects)
				{
					sharedObjects->share(drawCommands->children);
					sharedObjects->share(drawCommands);
				}
				// share the pipeline config and initialize it if it's unique
				if (sharedObjects)
					sharedObjects->share(graphicsPipelineConfig, [](auto gpc) { gpc->init(); });
				else
					graphicsPipelineConfig->init();
				auto stateGroup = vsg::StateGroup::create();
				graphicsPipelineConfig->copyTo(stateGroup, sharedObjects);
				// set up model transformation node
				/*auto transform = vsg::MatrixTransform::create();
				transform->subgraphRequiresLocalFrustum = false;*/
				auto transform = vsg::MatrixTransform::create();
				transform->setValue("guid", "Transform_" + node->guid);
				if (node->localMatrix.isZero())
				{
					transform->matrix = vsg::dmat4(); // 单位阵
				}
				else
				{
					transform->matrix = vsg::dmat4(node->localMatrix.data());
				}
				stateGroup->addChild(drawCommands);
				if (sharedObjects)
				{
					sharedObjects->share(stateGroup);
				}
				transform->addChild(stateGroup);
				if (sharedObjects)
				{
					sharedObjects->share(transform);
				}
				scenegraph->addChild(transform);
			}
		}
	}
	return scenegraph;
}