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
		//�o�q�O���F�ѨMindex���D
		VHandle vq, vw, vt, vr;
		vq	 = addVertex(Point(0, 0, 100));
		vw	 = addVertex(Point(1, 0, 100));
		vt	 = addVertex(Point(1, 1, 100));
		vr	 = addVertex(Point(0, 1, 100));
		addFace(vq, vw, vt, vr);
		/*----------------------------------------------------------------------*/
		/*�����ݭnsubdivision��face*/
		//��ltable
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
		/*��candidate faces��subdivision*/
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
		deleteFace(vq, vw, vt, vr);//�o��u�O���F�ѨMindex���D
		/*----------------------------------------------------------------------*/
		return 0;
	}
	int Model::quad_subdivision2(int _face_id)
	{
		/*----------------------------------------------------------------------*/
		//�o�q�O���F�ѨMindex���D
		VHandle vq, vw, vt, vr;
		vq	 = addVertex(Point(0, 0, 100));
		vw	 = addVertex(Point(1, 0, 100));
		vt	 = addVertex(Point(1, 1, 100));
		vr	 = addVertex(Point(0, 1, 100));
		addFace(vq, vw, vt, vr);
		/*----------------------------------------------------------------------*/
		/*�����ݭnsubdivision��face*/
		//��ltable
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
		/*��candidate faces��subdivision*/
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
		deleteFace(vq, vw, vt, vr);//�o��u�O���F�ѨMindex���D
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

const OpenMesh::VertexHandle Tri_Mesh::InvalidVertexHandle;

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
		for (fv_it = fv_iter(f_it); fv_it; ++fv_it) { // �C�ӭ����T���I
			// �C���I���T��vertexes
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

	//�s��mesh���I�M��
	std::vector<Tri_Mesh::VertexHandle> vhandle;
	std::vector<Tri_Mesh::VertexHandle>  face_vhandles;

	int verticesSeq[3] = { 0,0,0 };
	int verticesSeqPtr = 0;
	int isPatchHasPoint = 0;
	
	std::map<int, int> mapVerticesToVHandle;
	for (f_it = faces_begin(); f_it != faces_end(); ++f_it) {


		// �ˬd�o��face���S���Q�ڿ��
		int isFaceSelected = 0;
		for (int i = 0; i < selected.size(); i++) {
			if (f_it.handle().idx() == selected[i]) { // �����ܴN���U�@�B
				isFaceSelected = 1;
				break;
			}
			else if (f_it.handle().idx() != selected[i] && i == selected.size() - 1) isFaceSelected = 0; // �S���ܴN�����M��U�@��face
		}
		if (isFaceSelected == 0) continue;// �S���ܴN�����M��U�@��face
		face++;
		for (fv_it = fv_iter(f_it); fv_it; ++fv_it) { // �C�ӭ����T���I
			// �C���I���T��vertexes
			
			out_vertices.push_back(*(point(fv_it.handle()).data()));
			out_vertices.push_back(*(point(fv_it.handle()).data() + 1));
			out_vertices.push_back(*(point(fv_it.handle()).data() + 2));
			
			//============================================
			//not used
			//============================================
			/*int facepoint[3];
			int facepointPtr = 0;

			for (int i = out_vertices.size()-3; i < out_vertices.size(); i++) { // �C���ˬd�s�[�J���I���S���M�ª��I����
				int isSame = 0;
				for (int j = 0; j < out_vertices.size() && !isSame; j++) { // �q0�}�l�ˬd
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

			//�C�[�@���I�A�N�ˬd���S���Mpatch vhandle���I���ơA�S�����ܴN�[�Jpatch vhandle
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
			if (isPatchHasPoint == 0) { // �p�G�O�@�}�lpatch�S���I���ɭ�
				printf("first point...\n");
				vhandle.push_back(patch.add_vertex(Tri_Mesh::Point(out_vertices.at(0), out_vertices.at(1), out_vertices.at(2)))); // �Ĥ@���I�Oout_vertices��0,1,2==x,y,z
				verticesSeq[verticesSeqPtr++] = out_vertices.size() - 3; // �����O�bout_vertices�ĴX�Ӧ�m
				mapVerticesToVHandle[out_vertices.size() - 3] = patch.vertices_begin().handle().idx(); // �Nout_vertices��m�ഫ���spatch���I�Ǹ�
				printf("mapVToH = %d : %d\n", out_vertices.size() - 3, patch.vertices_begin().handle().idx());
			}
			if (isSame == 0 && isPatchHasPoint == 1) { // �p�G�O�s���I
				printf("new point...\n");
				vhandle.push_back( patch.add_vertex( Tri_Mesh::Point( out_vertices.at(out_vertices.size() - 3), out_vertices.at(out_vertices.size() - 2), out_vertices.at(out_vertices.size() - 1) ) ) );
				verticesSeq[verticesSeqPtr++] = out_vertices.size() - 3;
				mapVerticesToVHandle[out_vertices.size() - 3] = (v_it).handle().idx();
				printf("mapVToH = %d : %d\n", out_vertices.size() - 3, (v_it).handle().idx());
			}
			

		}

		//�C�]���@�ӭ����T���I����A�⭱�[�i�h
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
	for (int i = 0; i < out_vertices.size(); i+=3) { // �ˬd�s�[�J���I���S���M�ª��I����
		int isSame = 0;
		for (int j = 0; j < out_vertices.size() && !isSame; j+=3) { // �q0�}�l�ˬd
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
					// v_it�̭��x�s���I�n�p��Mvec3�����
					face_vhandles.push_back(vhandle[v_it.handle().idx()]);
				}
			}

			for (v_it = patch.vertices_begin(); v_it != patch.vertices_end(); v_it++) {
				if (out_vertices.at(verticesSeq[1]) == patch.point(v_it.handle())[0] && out_vertices.at(verticesSeq[1] + 1) == patch.point(v_it.handle())[1] && out_vertices.at(verticesSeq[1] + 2) == patch.point(v_it.handle())[2]) {
					// v_it�̭��x�s���I�n�p��Mvec3�����
					face_vhandles.push_back(vhandle[v_it.handle().idx()]);
				}
			}
			
			for (v_it = patch.vertices_begin(); v_it != patch.vertices_end(); v_it++) {
				if (out_vertices.at(verticesSeq[2]) == patch.point(v_it.handle())[0] && out_vertices.at(verticesSeq[2] + 1) == patch.point(v_it.handle())[1] && out_vertices.at(verticesSeq[2] + 2) == patch.point(v_it.handle())[2]) {
					// v_it�̭��x�s���I�n�p��Mvec3�����
					face_vhandles.push_back(vhandle[v_it.handle().idx()]);
				}
			}
			
			patch.add_face(face_vhandles);
			verticesSeqPtr = 0;
		}

	}*/
	//glPopAttrib();
}

void Tri_Mesh::getUV(std::vector<double> & patchuv, Tri_Mesh patch, float uvRotateAngle) {
	VVIter vv_it;
	VIter v_it;
	EIter e_it;
	HHandle heh;
	FVIter fv_it;
	FIter f_it;

	//Step1 : �����Ĥ@�����
	for (e_it = edges_begin(); e_it != edges_end(); e_it++) {
		printf("enter edge iterator...\n");
		
		bool isBoundary = patch.is_boundary(*e_it);

		if (isBoundary) {
			//patch.point(v_it.handle())[0];
			heh = patch.halfedge_handle(e_it.handle(), 1);
			/*Point v1 = patch.point(from_vertex_handle(h));
			Point v2 = patch.point(to_vertex_handle(h));
			printf("v1 = %d, %f, %f\n",v1[0], v1[1], v1[2]);
			printf("v2 = %f, %f, %f\n", v2.data(), v2.data() + 1, v2.data() + 2);*/
			break;
		}
	}

	// Step2 : �u�۸���ɧ�M�U�@����
	double perimeter = 0;
	std::vector<double> segLength;
	std::vector<Tri_Mesh::VertexHandle> vhs; // �x�s�ƧǦn������I
	HHandle hehNext = heh;
	do {
		Point from = patch.point(patch.from_vertex_handle(hehNext));
		Point to = patch.point(patch.to_vertex_handle(hehNext));
		perimeter += (from - to).length(); // v0 - v? ������
		printf("perimeter = %f\n", perimeter);
		segLength.push_back(perimeter); // �s�Jvector���A�H�K���ᰵtexcoord
		vhs.push_back(patch.from_vertex_handle(hehNext)); // ����ɤW���I�@�@�s�J
		hehNext = patch.next_halfedge_handle(hehNext); // �i�H�����̧ǩ��U�@��heh��
	} while (heh != hehNext);

	//Step3 : �䧹�Ҧ�������I�M�Z����A�i�H�N��ɤw���I����texcoord
	float rd = (225 + uvRotateAngle) * M_PI / 180.0;
	float initDist = 0;
	Tri_Mesh::TexCoord2D st(0, 0);
	float R = std::sqrt(2) / 2.0; // �ڸ�2/2
	st[0] = R * cos(rd) + 0.5;
	st[1] = R * sin(rd) + 0.5;

	if (st[0] > 1)
	{
		st[0] = 1;
		st[1] = tan(rd) / 2 + 0.5;
	}

	if (st[0] < 0)
	{
		st[0] = 0;
		st[1] = 0.5 - tan(rd) / 2;
	}

	if (st[1] > 1)
	{
		st[0] = tan(M_PI_2 - rd) / 2 + 0.5;
		st[1] = 1;
	}

	if (st[1] < 0)
	{
		st[0] = 0.5 - tan(M_PI_2 - rd) / 2;
		st[1] = 0;
	}


	if (uvRotateAngle <= 90)
	{
		initDist = st.length();
	}

	else if (uvRotateAngle <= 180)
	{
		initDist = 1 + st[1];
	}

	else if (uvRotateAngle <= 270)
	{
		initDist = 3 - st[0];
	}

	else
	{
		initDist = 4 - st[1];
	}


	patch.request_vertex_texcoords2D();
	patch.set_texcoord2D(vhs[0], st);
	perimeter /= 4.0;
	for (int i = 1; i < vhs.size(); ++i)
	{
		double curLen = segLength[i - 1] / perimeter + initDist; // �N�YL0n/Ltotal*4
		if (curLen > 4)
		{
			curLen -= 4;
		}

		if (curLen <= 1)
		{
			st[0] = curLen;
			st[1] = 0;
		}
		else if (curLen <= 2)
		{
			st[0] = 1;
			st[1] = curLen - 1;
		}
		else if (curLen <= 3)
		{
			st[0] = 3 - curLen;
			st[1] = 1;
		}
		else
		{
			st[0] = 0;
			st[1] = 4 - curLen;
		}

		patch.set_texcoord2D(vhs[i], st); // ���D�X����I��uv�y�СA�����w���I
	}


	//Step4 : ���U�ӡA��X�D��ɪ��v��(�ϥΧ��褽��)
	OpenMesh::HPropHandleT<double> heWeight;
	OpenMesh::VPropHandleT<int> row;
	patch.add_property(heWeight, "heWeight"); //�[�Jproperty�G�v��
	patch.add_property(row, "row");

	for (e_it = patch.edges_begin(); e_it != patch.edges_end(); ++e_it) {
		bool isBound = patch.is_boundary(*e_it);
		if (!isBound) { // �p�G���O��ɡA�N�n��L���v���A���half_edge���n
			GLdouble angle1, angle2, w;
			Tri_Mesh::HalfedgeHandle _heh = patch.halfedge_handle(e_it.handle(), 0);
			Point pFrom = patch.point(patch.from_vertex_handle(_heh)); // �_�I
			Point pTo = patch.point(patch.to_vertex_handle(_heh)); // ���I
			//���D�Gopposite_he_opposite_vh, opposite_vh���
			Point p1 = patch.point(patch.opposite_vh(_heh)); // �u�W���I
			Point p2 = patch.point(patch.opposite_he_opposite_vh(_heh)); // �u�U���I
			//auto _vh = patch.opposite_vh(_heh);
			double edgeLen = (pFrom - pTo).length(); //�u����
			OpenMesh::Vec3d v1 = (OpenMesh::Vec3d)(pTo - pFrom);
			v1.normalize();
			OpenMesh::Vec3d v2 = (OpenMesh::Vec3d)(p1 - pFrom);
			v2.normalize();
			angle1 = std::acos(OpenMesh::dot(v1, v2)); //�u�W�I�P�u��arccos�A���o����

			v2 = (OpenMesh::Vec3d)(p2 - pFrom);
			v2.normalize();

			angle2 = std::acos(OpenMesh::dot(v1, v2)); //�u�U�I�P�u��arccos�A���o����

			//���褽��
			w = (std::tan(angle1 / 2.0f) + std::tan(angle2 / 2.0f)) / edgeLen;
			patch.property(heWeight, _heh) = w;  //�N�䤤�@��edge��n��J�䪺�ʽ褤

			// �A�ӭp��Ϥ�V��halfedge weight
			v1 = -v1;
			v2 = (OpenMesh::Vec3d)(p1 - pTo);
			v2.normalize();
			angle1 = std::acos(OpenMesh::dot(v1, v2));//�u�W�I�P�u��arccos�A���o����

			v2 = (OpenMesh::Vec3d)(p2 - pTo);
			v2.normalize();
			angle2 = std::acos(OpenMesh::dot(v1, v2));//�u�U�I�P�u��arccos�A���o����

			w = (std::tan(angle1 / 2.0f) + std::tan(angle2 / 2.0f)) / edgeLen;
			patch.property(heWeight, patch.opposite_halfedge_handle(_heh)) = w;//�N�Ϥ�Vedge��n��J�䪺�ʽ褤
		}

	}

	//Step5 : ��X�x�}���j�p(NxN)�A�`�I��-����I
	int count = 0;
	for (v_it = patch.vertices_begin(); v_it != patch.vertices_end(); v_it++) { // debug�Gpatch.vertices_end()�n�[patch
		if (patch.is_boundary(*v_it)) {
			patch.property(row, *v_it) = -1;
		}
		else
		{
			patch.property(row, *v_it) = count++;
		}
	}

	//Step6 : ��g�x�}
	typedef Eigen::SparseMatrix<double> SpMat;
	SpMat A(count, count);
	Eigen::VectorXd BX(count);
	Eigen::VectorXd BY(count);//x �M y �U�Ѥ@��
	Eigen::SimplicialLDLT<Eigen::SparseMatrix<double> > linearSolver;

	BX.setZero();
	BY.setZero();
	// fiil matrix
	for ( v_it = patch.vertices_begin(); v_it != patch.vertices_end(); ++v_it)
	{
		if (!patch.is_boundary(*v_it)) // �����I�O�ڭ̭n�Ѫ��F��
		{
			int i = patch.property(row, *v_it);
			double totalWeight = 0;

			for (vv_it = patch.vv_iter(*v_it); vv_it.is_valid(); ++vv_it)
			{
				HHandle _heh = patch.find_halfedge(*v_it, *vv_it);
				double w = patch.property(heWeight, _heh);

				if (patch.is_boundary(*vv_it)) //�O��ɪ����w���A��JB�x�}
				{
					TexCoord2D texCoord = patch.texcoord2D(*vv_it);
					BX[i] += w * texCoord[0]; // �����ۥ[
					BY[i] += w * texCoord[1];
				}
				else // ���O��ɪ��������I�A��JA�x�}
				{
					int j = patch.property(row, *vv_it);
					A.insert(i, j) = -w;
				}
				totalWeight += w;
			}


			A.insert(i, i) = totalWeight; // �o�̪��Ѫk�������Ҧ���w���W�jW
		}
	}

	A.makeCompressed();

	//Step7 : �Ѷ}������uv�y�Э�
	SpMat At = A.transpose();
	linearSolver.compute(At * A);

	Eigen::VectorXd TX = linearSolver.solve(At * BX);
	Eigen::VectorXd TY = linearSolver.solve(At * BY);

	for ( v_it = patch.vertices_begin(); v_it != patch.vertices_end(); ++v_it)
	{
		if (!patch.is_boundary(*v_it))
		{
			int i = patch.property(row, *v_it);
			patch.set_texcoord2D(*v_it, TexCoord2D(TX[i], TY[i]));
		}
	}
	for (f_it = faces_begin(); f_it != faces_end(); ++f_it) {
		for (fv_it = fv_iter(f_it); fv_it; ++fv_it)
		{
			patchuv.push_back(patch.texcoord2D(*fv_it)[0]);
			patchuv.push_back(patch.texcoord2D(*fv_it)[1]);
			//patchuv.push_back(*(point(fv_it.handle()).data()));
			//patchuv.push_back(*(point(fv_it.handle()).data() + 1));

		}
	}
	printf("patchuv : \n");
	printf("patchuv.size() : %d\n", patchuv.size());
	for (int i = 0; i < patchuv.size(); i+=2) {
		printf("s = %f , t = %f\n", patchuv.at(i), patchuv.at(i+1));
	}

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
