#pragma once

#include <vgl.h>
#include "DotNetUtilities.h"
#include "Mesh/GUA_OM.h"
#include "Mesh/DP.h"
#include <LoadShaders.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/ext.hpp"
using namespace glm;

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#define FACE_SIZE 5000

GLint SCR_WIDTH = 817;
GLint SCR_HEIGHT = 541;

GLuint quadVAO;
GLuint quadVBO;
unsigned int framebuffer; // 這三個buffer是要做整張畫面的特效使用
unsigned int textureColorbuffer;
unsigned int rbo;
unsigned int programFrame;
glm::vec4 pixel;
int facesid[FACE_SIZE];
int facesptr = 0;
//-------------------------------
//framebuffer-shader ID
//-------------------------------
//--------------------------------------
//framebufferobject
//--------------------------------------
float quadVertices1[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f,  1.0f, 1.0f
};
std::string filename;

Tri_Mesh *mesh;
Tri_Mesh *patch; // 所選的片

bool isLoad = false;
std::vector<double> vertices;
std::vector<double> verticesPatch;

xform xf;
GLCamera camera;
float fov = 0.7f;

float eyeAngley = 0.0;
float eyedistance = 30.0;
#define DOR(angle) (angle*3.1415/180);

GLuint VBO;
GLuint VAO;
GLuint UBO;
int face;
int facePatch;

GLint posX, posY, posZ;

GLuint program;

GLint MatricesIdx;
GLuint ModelID;
GLuint ColorID;
GLuint xfID;


mat4 Projection;
mat4 ViewMatrix;
mat4 Model;
mat4 MVP;
mat4 Proj;
ShaderInfo shaders_robot[] = {
	{ GL_VERTEX_SHADER, "robotShader.vp" },//vertex shader
	{ GL_FRAGMENT_SHADER, "robotShader.fp" },//fragment shader
	{ GL_NONE, NULL } };


static const Mouse::button physical_to_logical_map[] = {
	Mouse::NONE, Mouse::ROTATE, Mouse::MOVEXY, Mouse::MOVEZ,
	Mouse::MOVEZ, Mouse::MOVEXY, Mouse::MOVEXY, Mouse::MOVEXY,
};
Mouse::button Mouse_State = Mouse::ROTATE;



namespace OpenMesh_EX {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// MyForm 的摘要
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form{
		public:
			MyForm(void){
				//constructer
				InitializeComponent();
				posX = 0;
				posY = 0;
				posZ = 0;
				std::cout << "construct" << std::endl;
			}

		protected:
			/// <summary>
			/// 清除任何使用中的資源。
			/// </summary>
			~MyForm(){
				if (components){
					delete components;
				}
			}

		private: System::Windows::Forms::MenuStrip^  menuStrip1;
		private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  loadModelToolStripMenuItem;
		private: System::Windows::Forms::OpenFileDialog^  openModelDialog;
		private: System::Windows::Forms::SaveFileDialog^  saveModelDialog;
		private: System::Windows::Forms::ToolStripMenuItem^  saveModelToolStripMenuItem;
		private: HKOGLPanel::HKOGLPanelControl^  hkoglPanelControl1;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::ComponentModel::IContainer^  components;
	protected:

		private:
			/// <summary>
			/// 設計工具所需的變數。
			/// </summary>


	#pragma region Windows Form Designer generated code
			/// <summary>
			/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改
			/// 這個方法的內容。
			/// </summary>
			void InitializeComponent(void)
			{
				this->components = (gcnew System::ComponentModel::Container());
				HKOGLPanel::HKCOGLPanelCameraSetting^  hkcoglPanelCameraSetting1 = (gcnew HKOGLPanel::HKCOGLPanelCameraSetting());
				HKOGLPanel::HKCOGLPanelPixelFormat^  hkcoglPanelPixelFormat1 = (gcnew HKOGLPanel::HKCOGLPanelPixelFormat());
				this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
				this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				this->loadModelToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				this->saveModelToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				this->openModelDialog = (gcnew System::Windows::Forms::OpenFileDialog());
				this->saveModelDialog = (gcnew System::Windows::Forms::SaveFileDialog());
				this->hkoglPanelControl1 = (gcnew HKOGLPanel::HKOGLPanelControl());
				this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
				this->menuStrip1->SuspendLayout();
				this->SuspendLayout();
				// 
				// menuStrip1
				// 
				this->menuStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
				this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->fileToolStripMenuItem });
				this->menuStrip1->Location = System::Drawing::Point(0, 0);
				this->menuStrip1->Name = L"menuStrip1";
				this->menuStrip1->Padding = System::Windows::Forms::Padding(8, 2, 0, 2);
				this->menuStrip1->Size = System::Drawing::Size(817, 27);
				this->menuStrip1->TabIndex = 1;
				this->menuStrip1->Text = L"menuStrip1";
				// 
				// fileToolStripMenuItem
				// 
				this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
					this->loadModelToolStripMenuItem,
						this->saveModelToolStripMenuItem
				});
				this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
				this->fileToolStripMenuItem->Size = System::Drawing::Size(45, 23);
				this->fileToolStripMenuItem->Text = L"File";
				// 
				// loadModelToolStripMenuItem
				// 
				this->loadModelToolStripMenuItem->Name = L"loadModelToolStripMenuItem";
				this->loadModelToolStripMenuItem->Size = System::Drawing::Size(168, 26);
				this->loadModelToolStripMenuItem->Text = L"Load Model";
				this->loadModelToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::loadModelToolStripMenuItem_Click);
				// 
				// saveModelToolStripMenuItem
				// 
				this->saveModelToolStripMenuItem->Name = L"saveModelToolStripMenuItem";
				this->saveModelToolStripMenuItem->Size = System::Drawing::Size(168, 26);
				this->saveModelToolStripMenuItem->Text = L"Save Model";
				this->saveModelToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::saveModelToolStripMenuItem_Click);
				// 
				// openModelDialog
				// 
				this->openModelDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MyForm::openModelDialog_FileOk);
				// 
				// saveModelDialog
				// 
				this->saveModelDialog->DefaultExt = L"obj";
				this->saveModelDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MyForm::saveModelDialog_FileOk);
				// 
				// hkoglPanelControl1
				// 
				hkcoglPanelCameraSetting1->Far = 1000;
				hkcoglPanelCameraSetting1->Fov = 45;
				hkcoglPanelCameraSetting1->Near = -1000;
				hkcoglPanelCameraSetting1->Type = HKOGLPanel::HKCOGLPanelCameraSetting::CAMERATYPE::ORTHOGRAPHIC;
				this->hkoglPanelControl1->Camera_Setting = hkcoglPanelCameraSetting1;
				this->hkoglPanelControl1->Dock = System::Windows::Forms::DockStyle::Fill;
				this->hkoglPanelControl1->Location = System::Drawing::Point(0, 27);
				this->hkoglPanelControl1->Margin = System::Windows::Forms::Padding(4);
				this->hkoglPanelControl1->Name = L"hkoglPanelControl1";
				hkcoglPanelPixelFormat1->Accumu_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
				hkcoglPanelPixelFormat1->Alpha_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
				hkcoglPanelPixelFormat1->Stencil_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
				this->hkoglPanelControl1->Pixel_Format = hkcoglPanelPixelFormat1;
				this->hkoglPanelControl1->Size = System::Drawing::Size(817, 541);
				this->hkoglPanelControl1->TabIndex = 2;
				this->hkoglPanelControl1->Load += gcnew System::EventHandler(this, &MyForm::hkoglPanelControl1_Load);
				this->hkoglPanelControl1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::hkoglPanelControl1_Paint);
				this->hkoglPanelControl1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseDown);
				this->hkoglPanelControl1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseMove);
				this->hkoglPanelControl1->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseWheel);
				// 
				// timer1
				// 
				this->timer1->Interval = 30;
				// 
				// MyForm
				// 
				this->AutoScaleDimensions = System::Drawing::SizeF(8, 15);
				this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				this->ClientSize = System::Drawing::Size(817, 568);
				this->Controls->Add(this->hkoglPanelControl1);
				this->Controls->Add(this->menuStrip1);
				this->MainMenuStrip = this->menuStrip1;
				this->Margin = System::Windows::Forms::Padding(4);
				this->Name = L"MyForm";
				this->Text = L"OpenMesh_EX";
				this->menuStrip1->ResumeLayout(false);
				this->menuStrip1->PerformLayout();
				this->ResumeLayout(false);
				this->PerformLayout();

			}
	#pragma endregion

		//init
		private: System::Void hkoglPanelControl1_Load(System::Object^  sender, System::EventArgs^  e){
			//std::cout << "init" << std::endl;
			//glutInitContextVersion(4, 3);//以OpenGL version4.3版本為基準
			//glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);//是否向下相容,GLUT_FORWARD_COMPATIBLE不支援(?
			//glutInitContextProfile(GLUT_CORE_PROFILE);
			//debug ： 選到的面怪怪的，寬和高在過程中被偷改了，重新復值
			SCR_WIDTH = this->hkoglPanelControl1->Size.Width;
			SCR_HEIGHT = this->hkoglPanelControl1->Size.Height;
			glewExperimental = GL_TRUE; //置於glewInit()之前
			if (glewInit()) {
				std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;//c error
				exit(EXIT_FAILURE);
			}
			else std::cout << "initialize GLEW success" << std::endl;//c error

			
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			
			/**/
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			//VAO
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			program = LoadShaders(shaders_robot);//讀取shader

			glUseProgram(program);//uniform參數數值前必須先use shader
			MatricesIdx = glGetUniformBlockIndex(program, "MatVP");
			ModelID = glGetUniformLocation(program, "Model");
			ColorID = glGetUniformLocation(program, "Color");
			xfID = glGetUniformLocation(program, "xf");

			Projection = glm::perspective(80.0f, float(SCR_WIDTH) / SCR_HEIGHT, 0.0f, 1000.0f);
			ViewMatrix = glm::lookAt(
				glm::vec3(0, 10, 25), // Camera is at (0,10,25), in World Space
				glm::vec3(0, 0, 0), // and looks at the origin
				glm::vec3(0, 1, 0)  // Head is up (set to 0,1,0 to look upside-down)
			);

			//UBO
			glGenBuffers(1, &UBO);
			glBindBuffer(GL_UNIFORM_BUFFER, UBO);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4) * 2, NULL, GL_DYNAMIC_DRAW);
			//get uniform struct size
			int UBOsize = 0;
			glGetActiveUniformBlockiv(program, MatricesIdx, GL_UNIFORM_BLOCK_DATA_SIZE, &UBOsize);
			//bind UBO to its idx
			glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, UBOsize);
			glUniformBlockBinding(program, MatricesIdx, 0);

			glClearColor(0.0, 0.0, 0.0, 1);//black screen

			//==================================================================================
			//用來存face id的frameBuffer
			//==================================================================================
			ShaderInfo shaderframeid[] = {
			{ GL_VERTEX_SHADER, "framebuffer.vp" },//vertex shader
			{ GL_FRAGMENT_SHADER, "framebuffer.fp" },//fragment shader
			{ GL_NONE, NULL } };
			programFrame = LoadShaders(shaderframeid);//讀取shader

			glUseProgram(programFrame);//uniform參數數值前必須先use shader

			// screen quad VAO
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);


			glUseProgram(programFrame);
			glUniform1i(glGetUniformLocation(programFrame, "screenTexture"), 0);

			// framebuffer configuration
			// -------------------------
			glGenFramebuffers(1, &framebuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			// create a color attachment texture

			glGenTextures(1, &textureColorbuffer);
			glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32UI, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA_INTEGER, GL_UNSIGNED_INT, NULL);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
			// 注意GL_RED和GL_UNSIGNED_INT
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
			// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)

			glGenRenderbuffers(1, &rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach						
																										  // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
			// Disable reading to avoid problems with older GPUs
			glReadBuffer(GL_NONE);
			GLenum dr[2] = { GL_COLOR_ATTACHMENT0 ,GL_DEPTH_ATTACHMENT };
			glDrawBuffers(2,dr);

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
				cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete attach!" << endl;
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			pixel.r = 0.0f;
			pixel.g = 0.0f;
			pixel.b = 0.0f;
			pixel.a = 0.0f;

			timer1->Enabled = true;
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

			for (int i = 0; i < FACE_SIZE; i++) {
				facesid[i] = -1;
			}
			facesptr = 0;
		}
		//display
		private: System::Void hkoglPanelControl1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e){
			std::cout << "refresh" << std::endl;

			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
			glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); // GLSL1.1 要重設
			glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
			glClearDepth(1.0);

			//glUseProgram(0); // GLSL1.1 要綁回0才畫得上去
			//glEnable(GL_COLOR_MATERIAL);
			glClearColor(1.0, 1.0, 1.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glPushMatrix();
			point center;
			center[0] = 0.0;
			center[1] = 0.0;
			center[2] = 0.0;
			//set camera
			camera.setupGL(xf * center, 1.0);
			//camera.autospin(xf);

			/*
			glPushMatrix();
			glMatrixMode(GL_MODELVIEW);
			glMultMatrixd((double *)xf);
			*/
			
			if (mesh != NULL) {
				//std::cout << "refresh mesh not null" << std::endl;
				//mesh->Render_SolidWireframe();
				glGenBuffers(1, &VBO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				std::cout << vertices[0] << std::endl;
				std::cout << vertices.size() << std::endl;

				glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(double), &vertices[0], GL_STATIC_DRAW);

			}

			glEnable(GL_DEPTH_TEST);
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glBindVertexArray(VAO);
			glUseProgram(program);//uniform參數數值前必須先use shader

			float eyey = DOR(eyeAngley);
			ViewMatrix = lookAt(
				glm::vec3(eyedistance*sin(eyey), 2, eyedistance*cos(eyey)), // Camera is at (0,0,20), in World Space
				glm::vec3(posX, posY, posZ), // and looks at the origin
				glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
			);

			MVP = make_mat4((double*)xf);
			GLdouble P[16];
			
			glGetDoublev(GL_PROJECTION_MATRIX, P);
			Proj = make_mat4((double*)P);
			//std::cout << "xf matrix : " << std::endl  << "---------"<< std::endl << xf << "-------" << std::endl;
			//std::cout << "xf after matrix : " << std::endl << "---------" << std::endl << to_string(MVP) << "-------" << std::endl;
			//std::cout << "v matrix : " << std::endl << "---------" << std::endl << to_string(ViewMatrix) << "-------" << std::endl;
			//std::cout << "p matrix : " << std::endl << "---------" << std::endl << &P << "-------" << std::endl;
			//update data to UBO for MVP
			glBindBuffer(GL_UNIFORM_BUFFER, UBO);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), &ViewMatrix);
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), &Proj);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			
			glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Model[0][0]);
			glUniformMatrix4fv(xfID, 1, GL_FALSE, &MVP[0][0]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			// 1rst attribute buffer : vertices
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0,				//location
				3,				//vec3
				GL_DOUBLE,			//type
				GL_FALSE,			//not normalized
				0,				//strip
				0);//buffer offset
			if (isLoad) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glm::vec3 color = glm::vec3(-1.0, 0.0, 0.0);
				glUniform3fv(ColorID, 1, &color[0]);
				glDrawArrays(GL_TRIANGLES, 0, face * 3);
				//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				//color = glm::vec3(0.0, 0.0, 0.0);
				//glUniform3fv(ColorID, 1, &color[0]);
				//glDrawArrays(GL_TRIANGLES, 0, face * 3);

			}
			glPopMatrix();

			/*//------------------------------------
			//framebufferobject--(因為原本畫在自己的framebuffer，回到原本的buffer才能畫到opengl螢幕)
			//-----------------------------------
			// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			//glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
									  // clear all relevant buffers
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (really necessery actually, since we will be able to see behind the quad anyways)
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 先清空主要buffer的存的pixel顏色
			//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glUseProgram(programFrame);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices1), &quadVertices1, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

			//screenShader.use();
			//glUseProgram(programFrame);
			//glBindVertexArray(quadVAO);
			glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
			glDrawArrays(GL_TRIANGLES, 0, 6);*/

			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			if (isLoad) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glm::vec3 color = glm::vec3(1.0, 0.85, 0.0);
				glUniform3fv(ColorID, 1, &color[0]);
				glDrawArrays(GL_TRIANGLES, 0, face * 3);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				color = glm::vec3(0.0, 0.0, 0.0);
				glUniform3fv(ColorID, 1, &color[0]);
				glDrawArrays(GL_TRIANGLES, 0, face * 3);
			}

			//----------------------------
			//畫出所選到的面(紅色)
			//---------------------------
			if (patch != NULL) {
				//std::cout << "refresh mesh not null" << std::endl;
				//mesh->Render_SolidWireframe();
				glGenBuffers(1, &VBO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				std::cout << verticesPatch[0] << std::endl;
				std::cout << verticesPatch.size() << std::endl;

				glBufferData(GL_ARRAY_BUFFER, verticesPatch.size() * sizeof(double), &verticesPatch[0], GL_STATIC_DRAW);
				printf("change the VBO to patch...\n");
			}
			glEnable(GL_DEPTH_TEST);
			glBindVertexArray(VAO);
			glUseProgram(program);//uniform參數數值前必須先use shader
			glBindBuffer(GL_UNIFORM_BUFFER, UBO);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), &ViewMatrix);
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), &Proj);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Model[0][0]);
			glUniformMatrix4fv(xfID, 1, GL_FALSE, &MVP[0][0]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0,				//location
				3,				//vec3
				GL_DOUBLE,			//type
				GL_FALSE,			//not normalized
				0,				//strip
				0);//buffer offset
			if (facesptr != 0) {
				printf("draw red patch...\n");
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glm::vec3 color = glm::vec3(1.0, 0.0, 0.0);
				glUniform3fv(ColorID, 1, &color[0]);
				glDrawArrays(GL_TRIANGLES, 0, facePatch * 3);
				//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				//color = glm::vec3(0.0, 0.0, 0.0);
				//glUniform3fv(ColorID, 1, &color[0]);
				//glDrawArrays(GL_TRIANGLES, 0, face * 3);

			}

		}

				 //mouseClick
		private: System::Void hkoglPanelControl1_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e){
			if (e->Button == System::Windows::Forms::MouseButtons::Left || e->Button == System::Windows::Forms::MouseButtons::Middle){
				//leftClick Or wheelClick
				point center;
				Mouse_State = Mouse::NONE;
				center[0] = 0.0;
				center[1] = 0.0;
				center[2] = 0.0;
				camera.mouse(e->X, e->Y, Mouse_State, xf * center, 1.0, xf);

				point depth;
				camera.read_depth(e->X, e->Y, depth);
				std::cout << "point : " << depth << std::endl;

				glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
				glReadBuffer(GL_COLOR_ATTACHMENT0);
				glReadPixels(e->X, SCR_HEIGHT - e->Y, 1, 1, GL_RGBA, GL_FLOAT, &pixel);
				printf("pixel %f to face id %f\n", pixel.r, pixel.g);
				printf("mouse x = %d mouse y = %d\n", e->X, SCR_HEIGHT - e->Y);
			
				// debug ： facesptr
				//將所選到的面id加入陣列中並且排序
				if(isLoad){
					
					for (int i = 0; i < facesptr; i++) {
						
						if (facesid[i] == int(pixel.r)) {
							break;
						}
						else if (i == facesptr - 1) {
							facesid[facesptr++] = int(pixel.r);
							break;
						}
					}
					if(facesptr == 0) facesid[facesptr++] = int(pixel.r);

					for (int k = 1; k < facesptr; k++) {
						int temp = facesid[k]; // ex ; arr[9]
						int j = k - 1; // j = 8
						while (temp <= facesid[j]) { // arr[9] <= arr[8]
							facesid[j + 1] = facesid[j];
							j -= 1;
						}
						facesid[j + 1] = temp;
					}


					printf("selected faces : ");
					for (int i = 0; i < facesptr; i++) {
						printf("%d ", facesid[i]);
						if (i == facesptr - 1) printf("\n");
					}
					printf("facesptr = %d\n", facesptr);
				}

				//----------------------------------
				//將所選到的面load到vector中
				//----------------------------------
				if (facesptr != 0) {
					//del old mesh on screen
					if (patch != NULL) delete patch;
					patch = new Tri_Mesh;
					//clear vertices and face to null
					verticesPatch.clear();
					verticesPatch.resize(0);
					std::cout << "verticesPatch.resize(0)" << verticesPatch.size() << std::endl;
					facePatch = 0;
					ReadFile(filename, patch); // change form here
					patch->loadToBufferPatch(verticesPatch, facePatch, facesid, facesptr);


					std::cout << "facePatch" << facePatch << std::endl;
					std::cout << "verticesPatch.size()" << verticesPatch.size() << std::endl;
				}
				
				glReadBuffer(GL_NONE);
				glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);


				hkoglPanelControl1->Invalidate();
			}
		}

				 //mouseDrag
		private: System::Void hkoglPanelControl1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e){
			if (e->Button == System::Windows::Forms::MouseButtons::Left){
				point center;
				Mouse_State = Mouse::ROTATE;
				center[0] = 0.0;
				center[1] = 0.0;
				center[2] = 0.0;
				camera.mouse(e->X, e->Y, Mouse_State,
					xf * center,
					1.0, xf);
				

				hkoglPanelControl1->Invalidate();
			}

			if (e->Button == System::Windows::Forms::MouseButtons::Middle){
				point center;
				Mouse_State = Mouse::MOVEXY;
				center[0] = 0.0;
				center[1] = 0.0;
				center[2] = 0.0;
				camera.mouse(e->X, e->Y, Mouse_State, xf * center, 1.0, xf);
				hkoglPanelControl1->Invalidate();
			}
		}

		//mouseWheel
		private: System::Void hkoglPanelControl1_MouseWheel(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e){
			if (e->Delta < 0){
				point center;
				Mouse_State = Mouse::WHEELDOWN;
				center[0] = 0.0;
				center[1] = 0.0;
				center[2] = 0.0;
				camera.mouse(e->X, e->Y, Mouse_State, xf * center, 1.0, xf);
				posZ++;
				//eyedistance--;
				hkoglPanelControl1->Invalidate();
			}
			else{
				point center;
				Mouse_State = Mouse::WHEELUP;
				center[0] = 0.0;
				center[1] = 0.0;
				center[2] = 0.0;
				camera.mouse(e->X, e->Y, Mouse_State, xf * center, 1.0, xf);
				posZ--;
				//eyedistance++;
				hkoglPanelControl1->Invalidate();
			}
		}

		//click "openModel"
		private: System::Void loadModelToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e){
			openModelDialog->Filter = "Model(*.obj)|*obj";
			openModelDialog->Multiselect = false;
			openModelDialog->ShowDialog();
		}

		//check openModel dialog
		private: System::Void openModelDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e){
			
			MarshalString(openModelDialog->FileName, filename);
			//del old mesh on screen
			if (mesh != NULL) delete mesh;
			mesh = new Tri_Mesh;
			//clear vertices and face to null
			vertices.clear();
			face = 0;
			if (ReadFile(filename, mesh)) std::cout << filename << std::endl;
			isLoad = true;
			mesh->loadToBuffer(vertices,face);
			std::cout << "face" << face << std::endl;
			hkoglPanelControl1->Invalidate();
		}

		//saveObj menu open
		private: System::Void saveModelToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e){
			saveModelDialog->Filter = "Model(*.obj)|*obj";
			saveModelDialog->ShowDialog();
		}

		//check saveObj in dialog 
		private: System::Void saveModelDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e){
			std::string filename;
			MarshalString(saveModelDialog->FileName, filename);
			if (SaveFile(filename, mesh)) std::cout << filename << std::endl;
		}
	};
}
