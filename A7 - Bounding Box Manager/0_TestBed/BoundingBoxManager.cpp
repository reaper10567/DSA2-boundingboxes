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
		m_vBoundingBox[nBox]->SetColor(MEWHITE);
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
				////Get Radius of Box1
				//float fRadius1 = m_vBoundingBox[nBox1]->GetRadius();
				//
				////Get Radius of Box2
				//float fRadius2 = m_vBoundingBox[nBox2]->GetRadius();
				//
				////Get origin of Box1
				//matrix4 mMatrix1 = m_vBoundingBox[nBox1]->GetModelMatrix();
				//vector3 vCentroid1 = m_vBoundingBox[nBox1]->GetCentroid();
				//vector3 fOrigin1 = static_cast<vector3>(glm::translate(mMatrix1, vCentroid1) * vector4(0.0f, 0.0f, 0.0f, 1.0f));
				//
				////Get origin of Box2
				//matrix4 mMatrix2 = m_vBoundingBox[nBox2]->GetModelMatrix();
				//vector3 vCentroid2 = m_vBoundingBox[nBox2]->GetCentroid();
				//vector3 fOrigin2 = static_cast<vector3>(glm::translate(mMatrix2, vCentroid2) * vector4(0.0f, 0.0f, 0.0f, 1.0f));
				//
				//float fDistance = glm::distance(fOrigin1,fOrigin2);
				//float fRadiusSum = fRadius1 + fRadius2;
				//if(fDistance < fRadiusSum)
				//{
				//	m_vCollidingNames.push_back(m_vBoundingBox[nBox1]->GetInstanceName());
				//	m_vCollidingNames.push_back(m_vBoundingBox[nBox2]->GetInstanceName());
				//}

				//set up block
				vector3 BB1Max = m_vBoundingBox[nBox1]->getOBBMax();
				vector3 BB1Min = m_vBoundingBox[nBox1]->getOBBMin();

				vector3 BB2Max = m_vBoundingBox[nBox2]->getOBBMax();
				vector3 BB2Min = m_vBoundingBox[nBox2]->getOBBMin();

				bool colliding = true;

				//Collision detection block
				/*
					start assuming the two are colliding
					Start by checking if the x position of the first box is within the bounds of the second box
					If so continue down 
					Else set colliding equal to false and break from the statement
					*this repeats for each case until either it is ruled out or is proven true*
				*/
				if(BB1Max.x > BB2Min.x && BB1Max.x < BB2Max.x){

					if(BB1Max.y > BB2Min.y && BB1Max.y < BB2Max.y){
						
						if(BB1Max.z > BB2Min.z && BB1Max.z < BB2Max.z){
							
						}
						else{
							colliding = false;
						}
					}

					else{
						colliding = false;
					}
				}

				else{
					colliding = false;
				}

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