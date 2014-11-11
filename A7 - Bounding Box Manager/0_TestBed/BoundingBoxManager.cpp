#include "BoundingBoxManager.h"

BoundingBoxManager* BoundingBoxManager::m_pInstance = nullptr;

BoundingBoxManager* BoundingBoxManager::GetInstance()
{
	if(m_pInstance == nullptr)
	{
		m_pInstance = new BoundingBoxManager();
	}
	return m_pInstance;
}

void BoundingBoxManager::ReleaseInstance()
{
	if(m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

void BoundingBoxManager::Init(void)
{
	m_pModelMngr = ModelManagerClass::GetInstance();
	m_vCollidingNames.clear();
	m_nBoxes = 0;
}

void BoundingBoxManager::Release(void)
{
	RemoveBox("ALL");
	return;
}
//The big 3
BoundingBoxManager::BoundingBoxManager(){Init();}
BoundingBoxManager::BoundingBoxManager(BoundingBoxManager const& other){ }
BoundingBoxManager& BoundingBoxManager::operator=(BoundingBoxManager const& other) { return *this; }
BoundingBoxManager::~BoundingBoxManager(){Release();};
//Accessors
int BoundingBoxManager::GetNumberOfBoxes(void){ return m_nBoxes; }

//--- Non Standard Singleton Methods
void BoundingBoxManager::SetOBBVisible(bool a_bVisible, String a_sInstance)
{
	if(a_sInstance == "ALL")
	{
		int nBoxes = GetNumberOfBoxes();
		for(int nBox = 0; nBox < nBoxes; nBox++)
		{
			m_vBoundingBox[nBox]->SetOBBVisible(a_bVisible);
		}
	}
	else
	{
		int nBox = m_pModelMngr->IdentifyInstance(a_sInstance);
		if(nBox < 0 || nBox < m_nBoxes)
			m_vBoundingBox[nBox]->SetOBBVisible(a_bVisible);
	}
}
void BoundingBoxManager::SetAABBVisible(bool a_bVisible, String a_sInstance)
{
	if(a_sInstance == "ALL")
	{
		int nBoxes = GetNumberOfBoxes();
		for(int nBox = 0; nBox < nBoxes; nBox++)
		{
			m_vBoundingBox[nBox]->SetAABBVisible(a_bVisible);
		}
	}
	else
	{
		int nBox = m_pModelMngr->IdentifyInstance(a_sInstance);
		if(nBox < 0 || nBox < m_nBoxes)
			m_vBoundingBox[nBox]->SetAABBVisible(a_bVisible);
	}
}

void BoundingBoxManager::SetColor(vector3 a_v3Color, String a_sInstance)
{
	if(a_sInstance == "ALL")
	{
		int nBoxes = GetNumberOfBoxes();
		for(int nBox = 0; nBox < nBoxes; nBox++)
		{
			m_vBoundingBox[nBox]->SetColor(a_v3Color);
		}
	}
	else
	{
		int nBox = m_pModelMngr->IdentifyInstance(a_sInstance);
		if(nBox < 0 || nBox < m_nBoxes)
			m_vBoundingBox[nBox]->SetColor(a_v3Color);
	}
}

void BoundingBoxManager::SetModelMatrix(matrix4 a_mModelMatrix, String a_sInstance)
{
	if(a_sInstance == "ALL")
	{
		int nBoxs = GetNumberOfBoxes();
		for(int nBox = 0; nBox < nBoxs; nBox++)
		{
			m_vBoundingBox[nBox]->SetModelMatrix(a_mModelMatrix);
		}
	}
	else
	{
		int nBox = m_pModelMngr->IdentifyInstance(a_sInstance);
		if(nBox < 0 || nBox < m_nBoxes)
			m_vBoundingBox[nBox]->SetModelMatrix(a_mModelMatrix);
	}
}

void BoundingBoxManager::RenderOBB(String a_sInstance)
{
	
	if(a_sInstance == "ALL")
	{
		int nBoxs = GetNumberOfBoxes();
		for(int nBox = 0; nBox < nBoxs; nBox++)
		{
			m_vBoundingBox[nBox]->RenderOBB(MEDEFAULT);
		}
	}
	else
	{
		int nBox = m_pModelMngr->IdentifyInstance(a_sInstance);
		if(nBox < 0 || nBox < m_nBoxes)
			m_vBoundingBox[nBox]->RenderOBB(MEDEFAULT);
	}
}
void BoundingBoxManager::RenderAABB(String a_sInstance)
{
	if(a_sInstance == "ALL")
	{
		int nBoxs = GetNumberOfBoxes();
		for(int nBox = 0; nBox < nBoxs; nBox++)
		{
			m_vBoundingBox[nBox]->RenderAABB(MEDEFAULT);
		}
	}
	else
	{
		int nBox = m_pModelMngr->IdentifyInstance(a_sInstance);
		if(nBox < 0 || nBox < m_nBoxes)
			m_vBoundingBox[nBox]->RenderAABB(MEDEFAULT);
	}
}

void BoundingBoxManager::AddBox(String a_sInstanceName)
{
	BoundingBoxClass* oBox = new BoundingBoxClass(a_sInstanceName);
	m_vBoundingBox.push_back(oBox);
	m_nBoxes ++;
}

void BoundingBoxManager::RemoveBox(String a_sInstanceName)
{
	if(a_sInstanceName == "ALL")
	{
		//Clear the vector's element first otherwise there is a memory leak
		for(int nBox = 0; nBox < m_nBoxes; nBox++)
		{
			BoundingBoxClass* pBS = m_vBoundingBox[nBox];
			delete pBS;
		}
		m_vBoundingBox.clear();
		m_nBoxes = 0;
		return;
	}
	std::vector<BoundingBoxClass*> vTemp;
	int nBox = m_pModelMngr->IdentifyInstance(a_sInstanceName);
	if(nBox < 0 || nBox < m_nBoxes)
	{
		for(int nBox = 0; nBox < m_nBoxes; nBox++)
		{
			if(nBox != nBox)
			{
				vTemp.push_back(m_vBoundingBox[nBox]);
			}
			else
			{
				BoundingBoxClass* pBS = m_vBoundingBox[nBox];
				delete pBS;
			}
		}
	}
	m_vBoundingBox = vTemp;
	m_nBoxes++;
}

void BoundingBoxManager::Update(void)
{
	m_vCollidingNames.clear();
	for(int nBox = 0; nBox < m_nBoxes; nBox++)
	{
		m_vBoundingBox[nBox]->SetColor(MEBLUE);
	}
	CollisionCheck();
	CollisionResponse();
}

void BoundingBoxManager::CollisionCheck(void)
{
	for(int nBox2 = 0; nBox2 < m_nBoxes; nBox2++)
	{
		for(int nBox1 = 0; nBox1 < m_nBoxes; nBox1++)
		{
			if(nBox1 != nBox2)
			{
				//set up block

				//Get the matrix and center of the first box
				//static cast those values in order to get the box's origin after the program begins running
				matrix4 mMatrix1 = m_vBoundingBox[nBox1]->GetModelMatrix();
				vector3 vCentroid1 = m_vBoundingBox[nBox1]->GetAABBCentroid();
				vector3 Origin1 = static_cast<vector3>(glm::translate(mMatrix1,vCentroid1) * vector4(0.0f,0.0f,0.0f,1.0f));

				//get the max values of the first
				//find the max values by adding the max values of each box to the origin point of the box
				vector3 BB1Max = m_vBoundingBox[nBox1]->getAABBMax();
				float BB1_Max_X =  BB1Max.x; 
				float BB1_Max_Y =  BB1Max.y; 
				float BB1_Max_Z =  BB1Max.z; 

				//repeat the same for the min values 
				vector3 BB1Min = m_vBoundingBox[nBox1]->getAABBMin();
				float BB1_Min_X =  BB1Min.x; 
				float BB1_Min_Y =  BB1Min.y; 
				float BB1_Min_Z =  BB1Min.z; 

				//repeat for the second box
				matrix4 mMatrix2 = m_vBoundingBox[nBox2]->GetModelMatrix();
				vector3 vCentroid2 = m_vBoundingBox[nBox2]->GetAABBCentroid();
				vector3 Origin2 = static_cast<vector3>(glm::translate(mMatrix2,vCentroid2)* vector4(0.0f,0.0f,0.0f,1.0f));

				vector3 BB2Max = m_vBoundingBox[nBox2]->getAABBMax();
				float BB2_Max_X =  BB2Max.x; 
				float BB2_Max_Y =  BB2Max.y; 
				float BB2_Max_Z =  BB2Max.z; 

				vector3 BB2Min = m_vBoundingBox[nBox2]->getAABBMin();
				float BB2_Min_X =  BB2Min.x; 
				float BB2_Min_Y =  BB2Min.y; 
				float BB2_Min_Z =  BB2Min.z; 

				//start them off as colliding
				bool colliding = true;

				//Collision detection block
				/*
					start assuming the two are colliding
					Start by checking if the x position of the first box is within the bounds of the second box
					If so continue down 
					Else set colliding equal to false and break from the statement
					*this repeats for each case until either it is ruled out or is proven true*
				*/
				//if the max value of the first box is less than the min value of the second
				//or the min value of the first is greater than the max value of the second, declare false
				//repeat for the x y and z cases
				if(BB1_Max_X < BB2_Min_X || BB1_Min_X > BB2_Max_X){colliding = false;}
				if(BB1_Max_Y < BB2_Min_Y || BB1_Min_Y > BB2_Max_Y){colliding = false;}
				if(BB1_Max_Z < BB2_Min_Z || BB1_Min_Z > BB2_Max_Z){colliding = false;}
				

				//Determine check to see if the collision detection passed or failed, and add names to list of colliding objects accordingly
				if(colliding)
				{
					m_vCollidingNames.push_back(m_vBoundingBox[nBox1]->GetInstanceName());
					m_vCollidingNames.push_back(m_vBoundingBox[nBox2]->GetInstanceName());
				}
			}
		}
	}
}

bool BoundingBoxManager::CheckForNameInList(String a_sName)
{
	int nNames = static_cast<int>(m_vCollidingNames.size());
	for(int nName = 0; nName < nNames; nName++)
	{
		if(m_vCollidingNames[nName] == a_sName)
			return true;
	}
	return false;
}

void BoundingBoxManager::CollisionResponse(void)
{
	for(int nBox = 0; nBox < m_nBoxes; nBox++)
	{
		if(CheckForNameInList(m_vBoundingBox[nBox]->GetInstanceName()))
			m_vBoundingBox[nBox]->SetColor(MERED);
	}
}