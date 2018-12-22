#include "GUA_OM.h"

namespace OMT
{
	/*======================================================================*/
	Model::Model()
	{
		request_vertex_status();
		request_edge_status();
		request_face_status();
	}
	Model::~Model()
	{
		release_vertex_status();
		release_edge_status();
		release_face_status();
	}
}
/*======================================================================*/
namespace OMP
{
	Model::Model()
	{
		Mesh.request_vertex_status();
		Mesh.request_edge_status();
		Mesh.request_face_status();
	}
	Model::~Model()
	{
		Mesh.release_vertex_status();
		Mesh.release_edge_status();
		Mesh.release_face_status();
	}
	/*======================================================================*/
	bool Model::ReadFile(std::string _fileName)
	{
		bool isRead = false;
		OpenMesh::IO::Options opt;
		if ( OpenMesh::IO::read_mesh(Mesh, _fileName, opt) )
		{
			//read mesh from filename OK!
			isRead = true;
		}
		if(isRead)
		{
			// If the file did not provide vertex normals and mesh has vertex normal ,then calculate them
			if (!opt.check( OpenMesh::IO::Options::VertexNormal ) && Mesh.has_vertex_normals())
			{
				Mesh.update_normals();
			}
		}
		return isRead;
	}
	bool Model::SaveFile(std::string _fileName)
	{
		bool isSave = false;
		OpenMesh::IO::Options opt;
		if ( OpenMesh::IO::write_mesh(Mesh, _fileName, opt) )
		{
			//read mesh from filename OK!
			isSave = true;
		}
		return isSave;
	}
	/*======================================================================*/
	void Model::Render_solid()
	{
		FIter f_it;
		FVIter	fv_it;
		glPushAttrib(GL_LIGHTING_BIT);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glEnable(GL_DEPTH_TEST);
		glPolygonOffset(2.0, 2.0);
		glBegin(GL_POLYGON);
		//glColor4f(1.0, 0.5, 1.0, 0.5);
		for (f_it = Mesh.faces_begin(); f_it != Mesh.faces_end(); ++f_it) 
		{
			for (fv_it = Mesh.fv_iter( f_it ); fv_it; ++fv_it)
			{						
				glNormal3dv(Mesh.normal(fv_it.handle()).data());
				glVertex3dv(Mesh.point(fv_it.handle()).data());
			}
		}
		glEnd();		
		glDisable(GL_POLYGON_OFFSET_FILL);
	}
	void Model::Render_wireframe()
	{
		MyMesh::HalfedgeHandle _hedge;
		EIter e_it=Mesh.edges_begin();

		glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glColor3f(0.0, 0.0, 0.0);
		glLineWidth(1);
		glBegin(GL_LINES);
		for(e_it=Mesh.edges_begin(); e_it != Mesh.edges_end(); ++e_it)
		{
			_hedge = Mesh.halfedge_handle(e_it.handle(),1);

			glVertex3dv(Mesh.point(Mesh.from_vertex_handle(_hedge)).data());
			glVertex3dv(Mesh.point(Mesh.to_vertex_handle(_hedge)).data());			
		}
		glEnd();
		glEnable(GL_LIGHTING);
	}
	/*======================================================================*/
	void Model::RenderSpecifiedPoint()
	{
		glPushAttrib(GL_LIGHTING_BIT);
		glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glPointSize(5.0f);
		glBegin(GL_POINTS);
		vector<sp_p>::iterator p_itr = sp_p_list.begin();
		for (p_itr; p_itr != sp_p_list.end(); ++p_itr)
		{
			glColor3f(p_itr->r, p_itr->g, p_itr->b);
			glVertex3dv(p_itr->pt.data());
		}
		glEnd();
		glEnable(GL_LIGHTING);
		glDisable(GL_POLYGON_OFFSET_FILL);
	}
	void Model::RenderSpecifiedVertex()
	{
		glPushAttrib(GL_LIGHTING_BIT);
		glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glPointSize(5.0f);
		glBegin(GL_POINTS);
		vector< sp_v >::iterator v_itr = sp_v_list.begin();
		for (v_itr; v_itr != sp_v_list.end(); ++v_itr)
		{
			glColor3f(v_itr->r, v_itr->g, v_itr->b);
			glVertex3dv(Mesh.point(v_itr->vh).data());
		}
		glEnd();
		glEnable(GL_LIGHTING);
		glDisable(GL_POLYGON_OFFSET_FILL);
	}
	void Model::RenderSpecifiedFace()
	{
		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glPushAttrib(GL_LIGHTING_BIT);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(0.5, 1.0);
		glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glBegin(GL_QUADS);
		FVIter fv_itr;
		vector< sp_f >::iterator f_itr;
		for (f_itr = sp_f_list.begin(); f_itr != sp_f_list.end(); ++f_itr)
		{
			glColor3f(f_itr->r, f_itr->g, f_itr->b);
			for (fv_itr=Mesh.fv_iter(f_itr->fh); fv_itr; ++fv_itr)
			{						
				glNormal3dv(Mesh.normal(fv_itr.handle()).data());
				glVertex3dv(Mesh.point(fv_itr.handle()).data());
			}
		}
		glEnd();		
		glEnable(GL_LIGHTING);
		glDisable(GL_POLYGON_OFFSET_FILL);
		glPolygonMode(GL_FRONT,GL_FILL);
		glEnable(GL_CULL_FACE);
	}

	/*======================================================================*/
	void Model::add_sp_p(Point   _p, float _r, float _g, float _b)
	{
		sp_p input_data;
		input_data.pt = _p;
		input_data.r = _r;
		input_data.g = _g;
		input_data.b = _b;
		sp_p_list.push_back(input_data);
	}
	void Model::add_sp_v(VHandle _v, float _r, float _g, float _b)
	{
		sp_v input_data;
		input_data.vh = _v;
		input_data.r = _r;
		input_data.g = _g;
		input_data.b = _b;
		sp_v_list.push_back(input_data);
	}
	void Model::add_sp_f(FHandle _f, float _r, float _g, float _b)
	{
		sp_f input_data;
		input_data.fh = _f;
		input_data.r = _r;
		input_data.g = _g;
		input_data.b = _b;
		sp_f_list.push_back(input_data);
	}
	void Model::clear_sp_p()
	{
		sp_p_list.clear();
	}
	void Model::clear_sp_v()
	{
		sp_v_list.clear();
	}
	void Model::clear_sp_f()
	{
		sp_f_list.clear();
	}
	/*======================================================================*/
	VHandle Model::addVertex(Point _p)
	{
		int find_result = findVertex(_p);
		if (find_result != -1)
		{
			return Mesh.vertex_handle(find_result);
		}
		else
		{
			return Mesh.add_vertex(_p);
		}
	}
	FHandle Model::addFace(VHandle _v0, VHandle _v1, VHandle _v2, VHandle _v3)
	{
		vector<VHandle> face_vhandles;

		face_vhandles.clear();
		face_vhandles.push_back(_v0);
		face_vhandles.push_back(_v1);
		face_vhandles.push_back(_v2);
		face_vhandles.push_back(_v3);
		return Mesh.add_face(face_vhandles);
	}
	void Model::deleteFace(FHandle _f)
	{
		Mesh.delete_face(_f);
		Mesh.garbage_collection();
	}
	void Model::deleteFace(VHandle _v0, VHandle _v1, VHandle _v2, VHandle _v3)
	{
		/* 
		v1				v0
		*<--------------*
		|				|
		|				|
		|				|
		|		f		|
		|				|
		|				|
		|				|
		* --------------*
		v2				v3
		*/

		HEHandle v0v1 = Mesh.find_halfedge(_v0, _v1);
		if (v0v1.is_valid())
		{
			FHandle fh = Mesh.face_handle(v0v1);
			if (fh.is_valid())
			{
				Mesh.delete_face(fh);
				Mesh.garbage_collection();
			}
		}
	}
	/*======================================================================*/
	bool Model::IsVertexVertex( VHandle _vj, VHandle _vi)
	{
		for( VVIter vvit = Mesh.vv_iter(_vi) ; vvit ; ++vvit )
			if( vvit.handle() == _vj )
				return true;
		return false;
	}
	/*======================================================================*/
	int Model::quad_subdivision1(int _face_id)
	{
		/*----------------------------------------------------------------------*/
		//這段是為了解決index問題
		VHandle vq, vw, vt, vr;
		vq	 = addVertex(Point(0, 0, 100));
		vw	 = addVertex(Point(1, 0, 100));
		vt	 = addVertex(Point(1, 1, 100));
		vr	 = addVertex(Point(0, 1, 100));
		addFace(vq, vw, vt, vr);
		/*----------------------------------------------------------------------*/
		/*收集需要subdivision的face*/
		//初始table
		bool *table = new bool[Mesh.n_faces()];
		for (unsigned i = 0; i < Mesh.n_faces(); i++)
		{
			table[i] = false;
		}

		vector< FHandle > candidate_faces, last_found_face;
		last_found_face.push_back(Mesh.face_handle(_face_id));
		table[_face_id] = true;

		while(last_found_face.size() != 0)
		{
			vector< FHandle > new_found_faces;
			for (vector< FHandle >::iterator crnt_f = last_found_face.begin(); crnt_f != last_found_face.end(); ++crnt_f)
			{
				for (FFIter ff_itr = Mesh.ff_iter(*crnt_f); ff_itr; ++ff_itr)
				{
					if (table[ff_itr.handle().idx()] != true)
					{
						new_found_faces.push_back(ff_itr.handle());
						table[ff_itr.handle().idx()] = true;
					}
				}
			}
			for (vector< FHandle >::iterator f_itr = last_found_face.begin(); f_itr != last_found_face.end(); ++f_itr)
			{
				candidate_faces.push_back(*f_itr);
			}
			last_found_face.assign(new_found_faces.begin(), new_found_faces.end());
		}
		delete table;
		/*----------------------------------------------------------------------*/
		/*對candidate faces做subdivision*/
		/*
			v0		vd		v3
			*-------*-------*
			|		|		|	 
			|		|		|
			|	  ve|		|
		  va*-------*-------*vc
			|		|		|
			|		|		|
			|		|		|
			*-------*-------*
			v1		vb		v2		
		*/
		for (vector< FHandle >::iterator face_itr = candidate_faces.begin(); face_itr != candidate_faces.end(); ++face_itr)
		{
			VHandle v[4], va, vb, vc, vd, ve;
			FVIter fv_itr = Mesh.fv_iter(*face_itr);
			for (int i = 0; fv_itr; ++fv_itr)
			{
				v[i++] = fv_itr.handle();
			}

			deleteFace(v[0], v[1], v[2], v[3]);
			va	 = addVertex((Mesh.point(v[0])+Mesh.point(v[1]))/2);
			vb	 = addVertex((Mesh.point(v[1])+Mesh.point(v[2]))/2);
			vc	 = addVertex((Mesh.point(v[2])+Mesh.point(v[3]))/2);
			vd	 = addVertex((Mesh.point(v[3])+Mesh.point(v[0]))/2);
			ve	 = addVertex((Mesh.point(v[0])+Mesh.point(v[1])+Mesh.point(v[2])+Mesh.point(v[3]))/4);
			addFace(vd, v[0], va, ve);
			addFace(va, v[1], vb, ve);
			addFace(vb, v[2], vc, ve);
			addFace(vc, v[3], vd, ve);
		}
		/*----------------------------------------------------------------------*/
		deleteFace(vq, vw, vt, vr);//這行只是為了解決index問題
		/*----------------------------------------------------------------------*/
		return 0;
	}
	int Model::quad_subdivision2(int _face_id)
	{
		/*----------------------------------------------------------------------*/
		//這段是為了解決index問題
		VHandle vq, vw, vt, vr;
		vq	 = addVertex(Point(0, 0, 100));
		vw	 = addVertex(Point(1, 0, 100));
		vt	 = addVertex(Point(1, 1, 100));
		vr	 = addVertex(Point(0, 1, 100));
		addFace(vq, vw, vt, vr);
		/*----------------------------------------------------------------------*/
		/*收集需要subdivision的face*/
		//初始table
		bool *table = new bool[Mesh.n_faces()];
		for (unsigned i = 0; i < Mesh.n_faces(); i++)
		{
			table[i] = false;
		}

		vector< FHandle > candidate_faces, last_found_face;
		last_found_face.push_back(Mesh.face_handle(_face_id));
		table[_face_id] = true;

		while(last_found_face.size() != 0)
		{
			vector< FHandle > new_found_faces;
			for (vector< FHandle >::iterator crnt_f = last_found_face.begin(); crnt_f != last_found_face.end(); ++crnt_f)
			{
				for (FFIter ff_itr = Mesh.ff_iter(*crnt_f); ff_itr; ++ff_itr)
				{
					if (table[ff_itr.handle().idx()] != true)
					{
						new_found_faces.push_back(ff_itr.handle());
						table[ff_itr.handle().idx()] = true;
					}
				}
			}
			for (vector< FHandle >::iterator f_itr = last_found_face.begin(); f_itr != last_found_face.end(); ++f_itr)
			{
				candidate_faces.push_back(*f_itr);
			}
			last_found_face.assign(new_found_faces.begin(), new_found_faces.end());
		}
		delete table;
		/*----------------------------------------------------------------------*/
		/*對candidate faces做subdivision*/
		/*
			v0		vh		vg		v3
			*-------*-------*-------*
			|		|		|		|
			|		|		|		|
			|	  vi|	  vl|		|
		 va *-------*-------*-------*vf
			|		|		|		|
			|		|		|		|
			|	  vj|	  vk|		|
		 vb *-------*-------*-------*ve
			|		|		|		|
			|		|		|		|
			|		|		|		|
			*-------*-------*-------*
			v1		vc		vd		v2		
		*/
		for (vector< FHandle >::iterator face_itr = candidate_faces.begin(); face_itr != candidate_faces.end(); ++face_itr){
			VHandle v[4], va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl;
			FVIter fv_itr = Mesh.fv_iter(*face_itr);
			for (int i = 0; fv_itr; ++fv_itr)
			{
				v[i++] = fv_itr.handle();
			}

			deleteFace(v[0], v[1], v[2], v[3]);
			va	 = addVertex((Mesh.point(v[0])*2+Mesh.point(v[1])  )/3);
			vb	 = addVertex((Mesh.point(v[0])  +Mesh.point(v[1])*2)/3);
			vc	 = addVertex((Mesh.point(v[1])*2+Mesh.point(v[2])  )/3);
			vd	 = addVertex((Mesh.point(v[1])  +Mesh.point(v[2])*2)/3);
			ve	 = addVertex((Mesh.point(v[2])*2+Mesh.point(v[3])  )/3);
			vf	 = addVertex((Mesh.point(v[2])  +Mesh.point(v[3])*2)/3);
			vg	 = addVertex((Mesh.point(v[3])*2+Mesh.point(v[0])  )/3);
			vh	 = addVertex((Mesh.point(v[3])  +Mesh.point(v[0])*2)/3);

			vi	 = addVertex((Mesh.point(vh)*2+Mesh.point(vc)  )/3);
			vj	 = addVertex((Mesh.point(vh)  +Mesh.point(vc)*2)/3);
			vk	 = addVertex((Mesh.point(vd)*2+Mesh.point(vg)  )/3);
			vl	 = addVertex((Mesh.point(vd)  +Mesh.point(vg)*2)/3);
			addFace(v[0], va, vi, vh);
			addFace(va, vb, vj, vi);
			addFace(vb, v[1], vc, vj);
			addFace(vc, vd, vk, vj);
			addFace(vd, v[2], ve, vk);
			addFace(ve, vf, vl, vk);
			addFace(vf, v[3], vg, vl);
			addFace(vg, vh, vi, vl);
			addFace(vi, vj, vk, vl);
		}
		/*----------------------------------------------------------------------*/
		deleteFace(vq, vw, vt, vr);//這行只是為了解決index問題
		/*----------------------------------------------------------------------*/
		return 0;
	}
	/*======================================================================*/
	int Model::findVertex(Point _p)
	{
		for( VIter v_itr = Mesh.vertices_begin(); v_itr != Mesh.vertices_end(); ++v_itr)
			if( Mesh.point(v_itr) == _p )
				return v_itr.handle().idx();
		return -1;
	}
	/*======================================================================*/
};
/*======================================================================*/
void Tri_Mesh::Render_Solid(){
		FIter f_it;
		FVIter	fv_it;
		//glPushAttrib(GL_LIGHTING_BIT);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glEnable(GL_LIGHTING);
		glPolygonOffset(2.0, 2.0);
		glBegin(GL_TRIANGLES);
		glColor4f(0.81, 0.74, 0.33, 0.3);
		for (f_it = faces_begin(); f_it != faces_end(); ++f_it) {
			for (fv_it = fv_iter( f_it ); fv_it; ++fv_it){						
				glNormal3dv(normal(fv_it.handle()).data());
				glVertex3dv(point(fv_it.handle()).data());
			}
		}
		glEnd();	
		
		glDisable(GL_POLYGON_OFFSET_FILL);
}

void Tri_Mesh::Render_SolidWireframe()
{
	FIter f_it;
	FVIter	fv_it;

	/*glPointSize(8.0);
	glBegin(GL_POINTS);
	glColor4f(1.0, 0.0, 0.0, 1.0);
	glVertex3dv(point(vertex_handle(0)));
	glEnd();*/
    glDisable(GL_LIGHTING);
	glPushAttrib(GL_LIGHTING_BIT);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glEnable(GL_DEPTH_TEST);
	glPolygonOffset(2.0, 2.0);
	glBegin(GL_TRIANGLES);
	glColor4f(1.0, 0.96, 0.49, 1.0);
	for (f_it = faces_begin(); f_it != faces_end(); ++f_it) {
		for (fv_it = fv_iter( f_it ); fv_it; ++fv_it){						
			//glNormal3dv(normal(fv_it.handle()));
			glVertex3dv(point(fv_it.handle()).data());
		}
	}
	glEnd();
	
	//glDisable(GL_POLYGON_OFFSET_FILL);

	glPushAttrib(GL_LIGHTING_BIT);	
	glDisable(GL_LIGHTING);
	glLineWidth(1.0);
	glColor3f(0.0, 0.0, 0.0);	
	glBegin(GL_LINES);
	for(OMT::EIter e_it = edges_begin(); e_it != edges_end(); ++e_it)
	{
		OMT::HEHandle _hedge = halfedge_handle(e_it.handle(),1);

		OMT::Point curVertex  = point(from_vertex_handle(_hedge));
		glVertex3dv(curVertex.data());

		curVertex = point(to_vertex_handle(_hedge));
		glVertex3dv(curVertex.data());			
	}
	glEnd();
	glPopAttrib();
}

void Tri_Mesh::loadToBuffer(std::vector<double> & out_vertices , int & face){
	//to buffer , then shader
	FIter f_it;
	FVIter	fv_it;

	face = 0;

	//glDisable(GL_LIGHTING);
	//glPushAttrib(GL_LIGHTING_BIT);
	//glEnable(GL_POLYGON_OFFSET_FILL);
	//glEnable(GL_DEPTH_TEST);
	//glPolygonOffset(6.0, 2.0);

	for (f_it = faces_begin(); f_it != faces_end(); ++f_it) {
		face++;
		for (fv_it = fv_iter(f_it); fv_it; ++fv_it) { // 每個面有三個點
			// 每個點有三個vertexes
			out_vertices.push_back(*(point(fv_it.handle()).data()));
			out_vertices.push_back(*(point(fv_it.handle()).data()+1));
			out_vertices.push_back(*(point(fv_it.handle()).data()+2));

			//std::cout << "push" << *(point(fv_it.handle()).data()) << "," << *(point(fv_it.handle()).data() + 1) << "," << *(point(fv_it.handle()).data()+2) << std::endl;

			//glNormal3dv(normal(fv_it.handle()));
			//glVertex3dv(point(fv_it.handle()).data());
		}
	}
	//glPopAttrib();
}

//typedef OpenMesh::PolyMesh_ArrayKernelT<>  MyMesh;
void Tri_Mesh::loadToBufferPatch(std::vector<double> & out_vertices, int & face, std::vector<int> selected, Tri_Mesh & patch) {
	//to buffer , then shader
	FIter f_it;
	FVIter fv_it;
	VIter v_it;
	
	face = 0;
	
	//glDisable(GL_LIGHTING);
	//glPushAttrib(GL_LIGHTING_BIT);
	//glEnable(GL_POLYGON_OFFSET_FILL);
	//glEnable(GL_DEPTH_TEST);
	//glPolygonOffset(6.0, 2.0);

	//新的mesh的點和面
	std::vector<Tri_Mesh::VertexHandle> vhandle;
	std::vector<Tri_Mesh::VertexHandle>  face_vhandles;

	int verticesSeq[3] = { 0,0,0 };
	int verticesSeqPtr = 0;
	int isPatchHasPoint = 0;
	//
	std::map<int, int> mapVerticesToVHandle;
	for (f_it = faces_begin(); f_it != faces_end(); ++f_it) {


		// 檢查這個face有沒有被我選到
		int isFaceSelected = 0;
		for (int i = 0; i < selected.size(); i++) {
			if (f_it.handle().idx() == selected[i]) { // 有的話就往下一步
				isFaceSelected = 1;
				break;
			}
			else if (f_it.handle().idx() != selected[i] && i == selected.size() - 1) isFaceSelected = 0; // 沒的話就直接套到下一個face
		}
		if (isFaceSelected == 0) continue;// 沒的話就直接套到下一個face
		face++;
		for (fv_it = fv_iter(f_it); fv_it; ++fv_it) { // 每個面有三個點
			// 每個點有三個vertexes
			
			out_vertices.push_back(*(point(fv_it.handle()).data()));
			out_vertices.push_back(*(point(fv_it.handle()).data() + 1));
			out_vertices.push_back(*(point(fv_it.handle()).data() + 2));
			
			//============================================
			//not used
			//============================================
			/*int facepoint[3];
			int facepointPtr = 0;

			for (int i = out_vertices.size()-3; i < out_vertices.size(); i++) { // 每次檢查新加入的點有沒有和舊的點重複
				int isSame = 0;
				for (int j = 0; j < out_vertices.size() && !isSame; j++) { // 從0開始檢查
					if (i == j) break;
					else if (out_vertices.at(i) == out_vertices.at(j)) {
						isSame = 1;
						break;
					}
					else if (j == out_vertices.size() - 1) {
						vhandle.push_back(patch.add_vertex(OMT::MyMesh::Point(out_vertices.at(i))));
						
					}
				}
			}*/
			//============================================
			//not used
			//============================================

			//每加一個點，就檢查有沒有和patch vhandle的點重複，沒有的話就加入patch vhandle
			isPatchHasPoint = 0;
			int isSame = 0;
			for (v_it = patch.vertices_begin(); v_it != patch.vertices_end(); v_it++) {
				isPatchHasPoint = 1;
				printf("v_iterator...\n");
				if((out_vertices.at(out_vertices.size() - 3) == patch.point(v_it.handle())[0] && out_vertices.at(out_vertices.size() - 2) == patch.point(v_it.handle())[1] && out_vertices.at(out_vertices.size() - 1) == patch.point(v_it.handle())[2])){
					verticesSeq[verticesSeqPtr++] = out_vertices.size() - 3;
					mapVerticesToVHandle[out_vertices.size() - 3] = v_it.handle().idx();
					isSame = 1;
					printf("isSame...\n");
					break;
				}
				
			}
			printf("isPatchHasPoint = %d\n", isPatchHasPoint);
			if (isPatchHasPoint == 0) { // 如果是一開始patch沒有點的時候
				printf("first point...\n");
				vhandle.push_back(patch.add_vertex(Tri_Mesh::Point(out_vertices.at(0), out_vertices.at(1), out_vertices.at(2)))); // 第一個點是out_vertices的0,1,2==x,y,z
				verticesSeq[verticesSeqPtr++] = out_vertices.size() - 3; // 紀錄是在out_vertices第幾個位置
				mapVerticesToVHandle[out_vertices.size() - 3] = patch.vertices_begin().handle().idx(); // 將out_vertices位置轉換成新patch的點序號
				printf("mapVToH = %d : %d\n", out_vertices.size() - 3, patch.vertices_begin().handle().idx());
			}
			if (isSame == 0 && isPatchHasPoint == 1) { // 如果是新的點
				printf("new point...\n");
				vhandle.push_back( patch.add_vertex( Tri_Mesh::Point( out_vertices.at(out_vertices.size() - 3), out_vertices.at(out_vertices.size() - 2), out_vertices.at(out_vertices.size() - 1) ) ) );
				verticesSeq[verticesSeqPtr++] = out_vertices.size() - 3;
				mapVerticesToVHandle[out_vertices.size() - 3] = (v_it).handle().idx();
				printf("mapVToH = %d : %d\n", out_vertices.size() - 3, (v_it).handle().idx());
			}
			

		}

		//每跑完一個面的三個點之後，把面加進去
		int num;
		face_vhandles.clear();
		num = mapVerticesToVHandle.find(verticesSeq[0])->second;
		printf("num = %d\n", num);
		face_vhandles.push_back(vhandle[num]);
		num = mapVerticesToVHandle.find(verticesSeq[1])->second;
		printf("num = %d\n", num);
		face_vhandles.push_back(vhandle[num]);
		num = mapVerticesToVHandle.find(verticesSeq[2])->second;
		printf("num = %d\n", num);
		face_vhandles.push_back(vhandle[num]);
		/*vhandle[0] = patch.add_vertex(Tri_Mesh::Point(-1, -1, 1));
		vhandle[1] = patch.add_vertex(Tri_Mesh::Point(1, -1, 1));
		vhandle[2] = patch.add_vertex(Tri_Mesh::Point(1, 1, 1));
		face_vhandles.push_back(vhandle[0]);
		face_vhandles.push_back(vhandle[1]);
		face_vhandles.push_back(vhandle[2]);*/
		printf("after add three point to face_vhandles...\n");
		patch.add_face(face_vhandles);
		verticesSeqPtr = 0;
	}

	/*int verticesSeq[3] = {0,0,0};
	int verticesSeqPtr = 0;
	for (int i = 0; i < out_vertices.size(); i+=3) { // 檢查新加入的點有沒有和舊的點重複
		int isSame = 0;
		for (int j = 0; j < out_vertices.size() && !isSame; j+=3) { // 從0開始檢查
			if (i == j) {
				printf("i == j\n");
				break;
			}
				//105,106,107
			else if (j == out_vertices.size() - 4 && (out_vertices.at(i) != out_vertices.at(j) || out_vertices.at(i + 1) != out_vertices.at(j + 1) || out_vertices.at(i + 2) != out_vertices.at(j + 2))) {
				
				vhandle.push_back(patch.add_vertex(OMT::MyMesh::Point(out_vertices.at(i), out_vertices.at(i+1), out_vertices.at(i+2))));
				verticesSeq[verticesSeqPtr++] = i;
				printf("new point add in mesh...\n");
				break;
			}
			else if (out_vertices.at(i) == out_vertices.at(j) && out_vertices.at(i+1) == out_vertices.at(j+1) && out_vertices.at(i+2) == out_vertices.at(j+2)) {
				isSame = 1;
				verticesSeq[verticesSeqPtr++] = i;
				printf("same point add to seq...\n");
				break;
			}
			
		}

		printf("none the point is added...\n");

		if (verticesSeqPtr == 3) {
			printf("add face to mesh...\n");
			face_vhandles.clear();
			face_vhandles.resize(0);
			for (v_it = patch.vertices_begin(); v_it != patch.vertices_end(); v_it++) {
				if (out_vertices.at(verticesSeq[0]) == patch.point(v_it.handle())[0] && out_vertices.at(verticesSeq[0] + 1) == patch.point(v_it.handle())[1] && out_vertices.at(verticesSeq[0] + 2) == patch.point(v_it.handle())[2]) {
					// v_it裡面儲存的點要如何和vec3做比較
					face_vhandles.push_back(vhandle[v_it.handle().idx()]);
				}
			}

			for (v_it = patch.vertices_begin(); v_it != patch.vertices_end(); v_it++) {
				if (out_vertices.at(verticesSeq[1]) == patch.point(v_it.handle())[0] && out_vertices.at(verticesSeq[1] + 1) == patch.point(v_it.handle())[1] && out_vertices.at(verticesSeq[1] + 2) == patch.point(v_it.handle())[2]) {
					// v_it裡面儲存的點要如何和vec3做比較
					face_vhandles.push_back(vhandle[v_it.handle().idx()]);
				}
			}
			
			for (v_it = patch.vertices_begin(); v_it != patch.vertices_end(); v_it++) {
				if (out_vertices.at(verticesSeq[2]) == patch.point(v_it.handle())[0] && out_vertices.at(verticesSeq[2] + 1) == patch.point(v_it.handle())[1] && out_vertices.at(verticesSeq[2] + 2) == patch.point(v_it.handle())[2]) {
					// v_it裡面儲存的點要如何和vec3做比較
					face_vhandles.push_back(vhandle[v_it.handle().idx()]);
				}
			}
			
			patch.add_face(face_vhandles);
			verticesSeqPtr = 0;
		}

	}*/
	//glPopAttrib();
}

void Tri_Mesh::Render_Wireframe(){
	//glPushAttrib(GL_LIGHTING_BIT);	
	glDisable(GL_LIGHTING);
	glLineWidth(1.0);
	
	glColor3f(0.0, 0.0, 0.0);	

	glBegin(GL_LINES);
	for(OMT::EIter e_it = edges_begin(); e_it != edges_end(); ++e_it)
	{
		OMT::HEHandle _hedge = halfedge_handle(e_it.handle(),1);

		OMT::Point curVertex  = point(from_vertex_handle(_hedge));
		glVertex3dv(curVertex.data());

		curVertex = point(to_vertex_handle(_hedge));
		glVertex3dv(curVertex.data());			
	}
	glEnd();
	
}

void Tri_Mesh::Render_Point()
{
	glPointSize ( 8.0 ) ;				  
	glColor3f( 1.0, 0.0, 0.0 ) ;
	glBegin(GL_POINTS);
	for (OMT::VIter v_it = vertices_begin() ; v_it != vertices_end() ; ++v_it)
	{
		  glVertex3dv(point(v_it).data());
	}
	glEnd();
}

bool ReadFile(std::string _fileName,Tri_Mesh *_mesh)
{
	bool isRead = false;
	OpenMesh::IO::Options opt;
	if ( OpenMesh::IO::read_mesh(*_mesh, _fileName, opt) )
	{
		//read mesh from filename OK!
		isRead = true;
	}
	if(isRead)
	{
		// If the file did not provide vertex normals and mesh has vertex normal ,then calculate them
		if (!opt.check( OpenMesh::IO::Options::VertexNormal ) && _mesh->has_vertex_normals())
		{
			_mesh->update_normals();
		}
	}
	return isRead;
}

bool SaveFile(std::string _fileName, Tri_Mesh *_mesh)
{
	bool isSave = false;
	if (OpenMesh::IO::write_mesh(*_mesh, _fileName))
	{
		isSave = true;
	}
	return isSave;
}
