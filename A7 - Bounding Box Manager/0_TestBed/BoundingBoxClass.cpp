#include "BoundingBoxClass.h"
//The big 3
BoundingBoxClass::BoundingBoxClass(String a_sInstanceName)
{
	//Initialize variables
	m_pAABBMesh = nullptr;
	m_pOBBMesh = nullptr;
	m_v3AABBCentroid = vector3(0.0f,0.0f,0.0f);
	m_v3OBBCentroid = vector3(0.0f,0.0f,0.0f);
	m_vAABBMax = vector3(0.0f,0.0f,0.0f);
	m_vAABBMin = vector3(0.0f,0.0f,0.0f);
	m_vOBBMax = vector3(0.0f,0.0f,0.0f);
	m_vOBBMin = vector3(0.0f,0.0f,0.0f);
	m_v3OBBColor = MEWHITE;
	m_v3AABBColor = MEBLUE;
	
	m_bOBBVisible = false;
	m_bAABBVisible = false;
	//Get the singleton instance of the Model Manager
	m_pModelMngr = ModelManagerClass::GetInstance();
	m_sInstance = a_sInstanceName;
	//Identify the instance from the list inside of the Model Manager
	int nInstance = m_pModelMngr->IdentifyInstance(m_sInstance);
	//If there is no instance with that name the Identify Instance will return -1
	//In which case there is nothing to do here so we just return without allocating memory
	if(nInstance == -1)
		return;

	m_mModelToWorld = m_pModelMngr->GetModelMatrix(m_sInstance);
	//Construct a sphere with the dimensions of the instance, they will be allocated in the
	//corresponding member variables inside the method
	m_pAABBMesh = new PrimitiveWireClass();

	m_pOBBMesh = new PrimitiveWireClass();
	CalculateOBBBox(m_sInstance);
	m_pOBBMesh->GenerateCube(1,MEWHITE);
	m_pOBBMesh->SetModelMatrix(glm::translate(m_mModelToWorld, m_v3OBBCentroid)*glm::scale(m_vOBBMax-m_vOBBMin));
	CalculateAABBBox(m_sInstance);
	//Get the Model to World matrix associated with the Instance
	
	//If the size of the radius is 0 it means that there are no points or all of them are allocated
	//right at the origin, which will cause an issue, so we just return with no allocations
	//Crete a new Sphere and initialize it using the member variables
	
	m_pAABBMesh->GenerateCube(1,MEWHITE);
	m_pAABBMesh->SetModelMatrix(glm::translate(m_mModelToWorld, m_v3AABBCentroid)*glm::scale(m_vAABBMax-m_vAABBMin));
	
}
BoundingBoxClass::BoundingBoxClass(BoundingBoxClass const& other)
{
	//Initialize the Sphere using other instance of it
	m_sInstance = other.m_sInstance;
	m_bOBBVisible = other.m_bOBBVisible;
	m_bAABBVisible = other.m_bAABBVisible;
	m_v3AABBCentroid = other.m_v3AABBCentroid;
	m_mModelToWorld = other.m_mModelToWorld;
	m_pModelMngr = other.m_pModelMngr;

	m_pAABBMesh = new PrimitiveWireClass();
	
	m_pOBBMesh = new PrimitiveWireClass();
	m_pOBBMesh->GenerateCube(1,MEWHITE);
	m_pOBBMesh->SetModelMatrix(glm::translate(m_mModelToWorld, m_v3OBBCentroid)*glm::scale(m_vOBBMax-m_vOBBMin));
	m_pAABBMesh->GenerateCube(1,MEWHITE);
	m_pAABBMesh->SetModelMatrix(glm::translate(m_mModelToWorld, m_v3AABBCentroid)*glm::scale(m_vAABBMax-m_vAABBMin));
	
	
}
BoundingBoxClass& BoundingBoxClass::operator=(BoundingBoxClass const& other)
{
	//If the incoming instance is the same as the current there is nothing to do here
	if(this != &other)
	{
		//Release the existing object
		Release();
		//Construct the object as in the copy constructor
		m_sInstance = other.m_sInstance;
		m_bOBBVisible = other.m_bOBBVisible;
		m_bAABBVisible = other.m_bAABBVisible;
		m_v3AABBCentroid = other.m_v3AABBCentroid;
		m_mModelToWorld = other.m_mModelToWorld;
		m_pModelMngr = other.m_pModelMngr;	

		m_pAABBMesh = new PrimitiveWireClass();
		

		m_pOBBMesh = new PrimitiveWireClass();
		m_pOBBMesh->GenerateCube(1,MEWHITE);
		m_pOBBMesh->SetModelMatrix(glm::translate(m_mModelToWorld, m_v3OBBCentroid)*glm::scale(m_vOBBMax-m_vOBBMin));
		m_pAABBMesh->GenerateCube(1,MEWHITE);
		m_pAABBMesh->SetModelMatrix(glm::translate(m_mModelToWorld, m_v3AABBCentroid)*glm::scale(m_vAABBMax-m_vAABBMin));
	
		
	}
	return *this;
}
BoundingBoxClass::~BoundingBoxClass()
{
	//Destroying the object requires releasing all the allocated memory first
	Release();
}
void BoundingBoxClass::Release(void)
{
	//If the mesh exists release it
	if(m_pAABBMesh != nullptr)
	{
		delete m_pAABBMesh;
		m_pAABBMesh = nullptr;
	}
	if(m_pOBBMesh != nullptr)
	{
		delete m_pOBBMesh;
		m_pOBBMesh = nullptr;
	}

	//The job of releasing the Model Manager Instance is going to be the work
	//of another method, we can't assume that this class is the last class
	//that uses this singleton, so we do not release it, we just make the pointer
	//point at nothing.
	m_pModelMngr = nullptr;
}
//Accessors
vector3 BoundingBoxClass::GetAABBCentroid(void){ return m_v3AABBCentroid; }
vector3 BoundingBoxClass::GetOBBCentroid(void) {return m_v3OBBCentroid;}
vector3 BoundingBoxClass::GetColor(void){ return m_v3OBBColor; }
void BoundingBoxClass::SetColor(vector3 a_v3Color){ m_v3OBBColor = a_v3Color; }
matrix4 BoundingBoxClass::GetModelMatrix(void){ return m_mModelToWorld; }
vector3 BoundingBoxClass:: getAABBMax(void){return m_vAABBMax;};
vector3 BoundingBoxClass:: getAABBMin(void){return m_vAABBMin;};
vector3 BoundingBoxClass:: getOBBMax(void){return m_vOBBMax;};
vector3 BoundingBoxClass:: getOBBMin(void){return m_vOBBMin;};
void BoundingBoxClass::SetModelMatrix(matrix4 a_mModelMatrix)
{
	//Sets the model matrix of the Sphere
	m_mModelToWorld = a_mModelMatrix;
	//Sets the Model Matrix of the actual Box shape
	//(which is translated m_v3Centrod away from the origin of our box)
	m_pOBBMesh->SetModelMatrix(glm::translate(a_mModelMatrix, m_v3OBBCentroid)*glm::scale(m_vOBBMax-m_vOBBMin));
	CalculateAABBBox(m_sInstance);
	m_pAABBMesh->SetModelMatrix(glm::translate(a_mModelMatrix, m_v3AABBCentroid)*glm::scale(m_vAABBMax-m_vAABBMin));
	
}
bool BoundingBoxClass::GetOBBVisible(void) { return m_bOBBVisible; }
void BoundingBoxClass::SetOBBVisible(bool a_bVisible) { m_bOBBVisible = a_bVisible; }
bool BoundingBoxClass::GetAABBVisible(void) { return m_bAABBVisible;};
void BoundingBoxClass::SetAABBVisible(bool a_bVisible){m_bAABBVisible = a_bVisible;};
String BoundingBoxClass::GetInstanceName(void){ return m_sInstance; }
//Methods
void BoundingBoxClass::CalculateAABBBox(String a_sInstance)
{
	//CalculateOBBBox(a_sInstance);
	//Get the vertices List to calculate the maximum and minimum
	
	std::vector<vector3> vVertices = m_pModelMngr->GetVertices(a_sInstance);
	int nVertices = static_cast<int>(vVertices.size());
	
	//If the size of the List is 0 it means we dont have any vertices in this Instance
	//which means there is an error somewhere
	if(nVertices == 0)
		return;

	//Go one by one on each component and realize which one is the smallest one
	vector3 v3Minimum;
	if(nVertices > 0)
	{
		//We assume the first vertex is the smallest one
		v3Minimum = vVertices[0];
		//And iterate one by one
		for(int nVertex = 1; nVertex < nVertices; nVertex++)
		{
			if(vVertices[nVertex].x < v3Minimum.x)
				v3Minimum.x = vVertices[nVertex].x;

			if(vVertices[nVertex].y < v3Minimum.y)
				v3Minimum.y = vVertices[nVertex].y;

			if(vVertices[nVertex].z < v3Minimum.z)
				v3Minimum.z = vVertices[nVertex].z;
		}
	}
	
	//Go one by one on each component and realize which one is the largest one
	vector3 v3Maximum;
	if(nVertices > 0)
	{
		//We assume the first vertex is the largest one
		v3Maximum = vVertices[0];
		//And iterate one by one
		for(int nVertex = 1; nVertex < nVertices; nVertex++)
		{
			if(vVertices[nVertex].x > v3Maximum.x)
				v3Maximum.x = vVertices[nVertex].x;

			if(vVertices[nVertex].y > v3Maximum.y)
				v3Maximum.y = vVertices[nVertex].y;

			if(vVertices[nVertex].z > v3Maximum.z)
				v3Maximum.z = vVertices[nVertex].z;
		}
	}
	//v3Maximum *= m_pModelMngr->GetModelMatrix(a_sInstance);
	//v3Minimum *= m_pModelMngr->GetModelMatrix(a_sInstance);
	m_vAABBMax = v3Maximum;
	m_vAABBMin = v3Minimum;
	//The centroid is going to be the point that is halfway of the min to the max
	m_v3AABBCentroid = v3Minimum + v3Maximum;
	m_v3AABBCentroid /= 2.0f;
	
	
	//SetModelMatrix(glm::scale(m_v3AABBCentroid));
	return;
}

void BoundingBoxClass::CalculateOBBBox(String a_sInstance)
{
	//Get the vertices List to calculate the maximum and minimum
	std::vector<vector3> vVertices = m_pModelMngr->GetVertices(a_sInstance);
	int nVertices = static_cast<int>(vVertices.size());
	
	//If the size of the List is 0 it means we dont have any vertices in this Instance
	//which means there is an error somewhere
	if(nVertices == 0)
		return;

	//Go one by one on each component and realize which one is the smallest one
	vector3 v3Minimum;
	if(nVertices > 0)
	{
		//We assume the first vertex is the smallest one
		v3Minimum = vVertices[0];
		//And iterate one by one
		for(int nVertex = 1; nVertex < nVertices; nVertex++)
		{
			if(vVertices[nVertex].x < v3Minimum.x)
				v3Minimum.x = vVertices[nVertex].x;

			if(vVertices[nVertex].y < v3Minimum.y)
				v3Minimum.y = vVertices[nVertex].y;

			if(vVertices[nVertex].z < v3Minimum.z)
				v3Minimum.z = vVertices[nVertex].z;
		}
	}
	
	//Go one by one on each component and realize which one is the largest one
	vector3 v3Maximum;
	if(nVertices > 0)
	{
		//We assume the first vertex is the largest one
		v3Maximum = vVertices[0];
		//And iterate one by one
		for(int nVertex = 1; nVertex < nVertices; nVertex++)
		{
			if(vVertices[nVertex].x > v3Maximum.x)
				v3Maximum.x = vVertices[nVertex].x;

			if(vVertices[nVertex].y > v3Maximum.y)
				v3Maximum.y = vVertices[nVertex].y;

			if(vVertices[nVertex].z > v3Maximum.z)
				v3Maximum.z = vVertices[nVertex].z;
		}
	}
	m_vOBBMax = v3Maximum;
	m_vOBBMin = v3Minimum;
	//The centroid is going to be the point that is halfway of the min to the max
	m_v3OBBCentroid = v3Minimum + v3Maximum;
	m_v3OBBCentroid /= 2.0f;
	
	
	//SetModelMatrix(glm::scale(m_v3OBBCentroid));
	return;
}

void BoundingBoxClass::RenderOBB( vector3 a_vColor )
{
	//If the shape is visible render it
	//otherwise just return
	if(!m_bOBBVisible)
		return;
	//Calculate the color we want the shape to be
	vector3 vColor;
	//if the argument was MEDEFAULT just use the color variable in our class
	if(a_vColor == MEDEFAULT)
		vColor = m_v3OBBColor;
	else //Otherwise use the argument
		vColor = a_vColor;

	//render the shape using a special case of the Shape Render method which uses the Color Shader.
	m_pOBBMesh->Render( matrix4(1.0f), vColor );
	
}

void BoundingBoxClass::RenderAABB( vector3 a_vColor )
{
	//If the shape is visible render it
	//otherwise just return
	if(!m_bAABBVisible)
		return;
	//Calculate the color we want the shape to be
	vector3 vColor;
	//if the argument was MEDEFAULT just use the color variable in our class
	if(a_vColor == MEDEFAULT)
		vColor = m_v3AABBColor;
	else //Otherwise use the argument
		vColor = a_vColor;

	//render the shape using a special case of the Shape Render method which uses the Color Shader.
	m_pAABBMesh->Render( matrix4(1.0f), vColor );
	
}