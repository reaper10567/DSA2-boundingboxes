#ifndef _BOUNDINGBOXMANAGER_H_
#define _BOUNDINGBOXMANAGER_H_

#include "BoundingBoxClass.h"

using namespace MyEngine;

//System Class
class BoundingBoxManager
{
	int m_nBoxes; //Number of Boxes in the List
	std::vector<BoundingBoxClass*> m_vBoundingBox; //List of Boxes in the manager
	ModelManagerClass* m_pModelMngr;//Model manager instance
	std::vector<String> m_vCollidingNames;//List of Names that are currently colliding
public:
	static BoundingBoxManager* GetInstance(); // Singleton accessor
	/*Release the Singleton */
	void ReleaseInstance(void);
	/*Get the number of Boxes in the manager */
	int GetNumberOfBoxes(void);
	/*Add a Box to the manager
	Arguments:
		a_sInstanceName name of the instance to create a Box from */
	void AddBox(String a_sInstanceName);
	/*Remove a Box from the List in the manager
	Arguments:
		a_sInstanceName name of the instance to remove from the List */
	void RemoveBox(String a_sInstanceName = "ALL");
	/*Sets the visibility of the specified Instance
	Arguments:
		a_bVisible sets whether the shape will be drawn or not
		a_sInstanceName identify the shape if ALL is provided then it applies to all shapes*/
	void SetOBBVisible(bool a_bVisible, String a_sInstanceName = "ALL");
	void SetAABBVisible(bool a_bVisible, String a_sInstanceName = "ALL");
	/*Sets the Color of the specified Instance
	Arguments:
		a_v3Color sets the color of the shape to be drawn
		a_sInstanceName identify the shape if ALL is provided then it applies to all shapes*/
	void SetColor(vector3 a_v3Color, String a_sInstanceName = "ALL");
	/*Sets the Model matrix to the object and the shape
	Arguments:
		a_mModelMatrix matrix4 that contains the space provided
		a_sInstanceName identify the shape if ALL is provided then it applies to all shapes*/
	void SetModelMatrix(matrix4 a_mModelMatrix, String a_sInstanceName = "ALL");
	/*Render the specified shape
	Arguments:
		a_sInstanceName identify the shape if ALL is provided then it applies to all shapes*/
	void RenderOBB(String a_sInstanceName = "ALL");
	void RenderAABB(String a_sInstanceName = "ALL");
	/*Initializes the list of names and check collision and collision resolution*/
	void Update(void);

private:
	/*Constructor*/
	BoundingBoxManager(void);
	/*Copy Constructor*/
	BoundingBoxManager(BoundingBoxManager const& other);
	/*Copy Assignment operator*/
	BoundingBoxManager& operator=(BoundingBoxManager const& other);
	/*Destructor*/
	~BoundingBoxManager(void);
	/*Releases the allocated memory*/
	void Release(void);
	/*Initializes the manager*/
	void Init(void);
	
	static BoundingBoxManager* m_pInstance; // Singleton
	/*Check the collision within all Boxes*/
	void CollisionCheck(void);
	/*Response to the collision test*/
	void CollisionResponse(void);
	/*Checks whether a name is the List of collisions
	Arguments
		a_sName checks this specific name*/
	bool CheckForNameInList(String a_sName);
};

#endif