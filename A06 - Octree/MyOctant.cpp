#include "MyOctant.h"
using namespace Simplex;

//static
uint MyOctant::m_uOctantCount;
uint MyOctant::m_uLeafCount;
uint MyOctant::m_uMaxLevel;
uint MyOctant::m_uIdealEntityCount;

//root construct 
MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	//create root
	Init();
	m_pRoot = this;
	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;

	//sets the max/min to center
	m_v3Max = m_v3Min = m_pEntityMngr->GetRigidBody()->GetCenterGlobal();
	m_uCurrEntityCount = m_pEntityMngr->GetEntityCount();

	for (uint i = 0; i < m_uCurrEntityCount; ++i)
	{
		m_lEntityList.push_back(i);

		//get min/max
		vector3 min = m_pEntityMngr->GetRigidBody(i)->GetMinGlobal();
		vector3 max = m_pEntityMngr->GetRigidBody(i)->GetMaxGlobal();

		//set min/max
		if (min.x < m_v3Min.x) m_v3Min.x = min.x;
		if (min.y < m_v3Min.y) m_v3Min.y = min.y;
		if (min.z < m_v3Min.z) m_v3Min.z = min.z;
		if (max.x > m_v3Max.x) m_v3Max.x = max.x;
		if (max.y > m_v3Max.y) m_v3Max.y = max.y;
		if (max.z > m_v3Max.z) m_v3Max.z = max.z;
	}

	//calc center
	m_v3Center = (m_v3Min + m_v3Max) / 2.0f;
	m_v3Size = m_v3Max - m_v3Min;

	//create children
	Subdivide();

	//add leaf dimensions
	ConfigureDimensions();
}

//constructor branch/leaf
MyOctant::MyOctant(vector3 a_v3Center, vector3 a_v3Size)
{
	Init();
	m_v3Center = a_v3Center;
	m_v3Size = a_v3Size;

	m_v3Max = a_v3Center + m_v3Size / 2.0f;
	m_v3Min = a_v3Center - m_v3Size / 2.0f;
}

//copy con
MyOctant::MyOctant(MyOctant const & other)
{
	Init();

	//copy data
	m_uLevel = other.m_uLevel;
	m_v3Size = other.m_v3Size;
	m_v3Center = other.m_v3Center;
	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;
	m_pParent = other.m_pParent;

	//copy/create more nodes
	m_uChildren = other.m_uChildren;
	for (uint i = 0; i < m_uChildren; ++i)
	{
		m_pChild[i] = new MyOctant(*other.m_pChild[i]);
	}

	//create entity list
	m_uCurrEntityCount = other.m_uCurrEntityCount;
	for (uint i = 0; i < m_uCurrEntityCount; ++i)
	{
		m_lEntityList.push_back(other.m_lEntityList[i]);
	}

	//if root copy child
	m_pRoot = other.m_pRoot;

	if (this == m_pRoot)
	{
		float fChildCount = other.m_lChildren.size();
		for (uint i = 0; i < fChildCount; ++i)
		{
			m_lChildren.push_back(other.m_lChildren[i]);
		}
	}
}

//copy assignment
MyOctant & MyOctant::operator=(MyOctant const & other)
{
	if (&other == this)
	{
		return *this;
	}

	Release();
	Init();

	//copy data
	m_uLevel = other.m_uLevel;
	m_v3Size = other.m_v3Size;
	m_v3Center = other.m_v3Center;
	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;
	m_pParent = other.m_pParent;

	//copy/create more nodes
	m_uChildren = other.m_uChildren;
	for (uint i = 0; i < m_uChildren; ++i)
	{
		m_pChild[i] = new MyOctant(*other.m_pChild[i]);
	}

	//create entity list
	m_uCurrEntityCount = other.m_uCurrEntityCount;
	for (uint i = 0; i < m_uCurrEntityCount; ++i)
	{
		m_lEntityList.push_back(other.m_lEntityList[i]);
	}

	//if root copy child
	m_pRoot = other.m_pRoot;
	if (this == m_pRoot)
	{
		float fChildCount = other.m_lChildren.size();

		for (uint i = 0; i < fChildCount; ++i)
		{
			m_lChildren.push_back(other.m_lChildren[i]);
		}
	}

	return *this;
}

//destructor
MyOctant::~MyOctant(void)
{
	Release();
}

void MyOctant::Swap(MyOctant & other)
{
	std::swap(m_uID, other.m_uID);
	std::swap(m_uLevel, other.m_uLevel);
	std::swap(m_uChildren, other.m_uChildren);
	std::swap(m_v3Size, other.m_v3Size);
	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Max, other.m_v3Max);
	std::swap(m_pParent, other.m_pParent);
	std::swap(m_pChild, other.m_pChild);
	std::swap(m_lEntityList, other.m_lEntityList);
	std::swap(m_uCurrEntityCount, other.m_uCurrEntityCount);
	std::swap(m_pRoot, other.m_pRoot);
	std::swap(m_lChildren, other.m_lChildren);
}

//Getterz
vector3 MyOctant::GetSize() { return m_v3Size; }

vector3 MyOctant::GetCenterGlobal() { return m_v3Center; }

vector3 MyOctant::GetMinGlobal() { return m_v3Min; }

vector3 MyOctant::GetMaxGlobal() { return m_v3Max; }

uint MyOctant::GetOctantCount() { return m_uOctantCount; }

uint Simplex::MyOctant::GetLeafCount() { return m_uLeafCount; }

MyOctant * MyOctant::GetParent() { return m_pParent; }

///////

MyOctant * MyOctant::GetChild(uint a_nChild)
{
	if (m_uChildren == 0)
		return nullptr;
	else return m_pChild[a_nChild];
}

bool MyOctant::IsLeaf(void) { return m_uChildren == 0; }

bool MyOctant::ContainsMoreThan(uint a_nEntities) { return m_uCurrEntityCount > a_nEntities; }

bool MyOctant::IsColliding(uint a_uRBIndex)
{
	MyRigidBody* rb = m_pEntityMngr->GetRigidBody(a_uRBIndex);
	vector3 rb_max = rb->GetMaxGlobal();
	vector3 rb_min = rb->GetMinGlobal();
	if (rb_max.x > m_v3Min.x &&
		rb_max.y > m_v3Min.y &&
		rb_max.z > m_v3Min.z &&
		rb_min.x < m_v3Max.x &&
		rb_min.y < m_v3Max.y &&
		rb_min.z < m_v3Max.z)
	{
		return true;
	}
	else return false;
}

//display select octants
void Simplex::MyOctant::Display(uint a_uIndex, vector3 a_v3Color)
{
	if (a_uIndex >= m_uOctantCount)
	{

		DisplayAll();
		return;
	}

	m_lChildren[a_uIndex]->DisplayCurrent(a_v3Color);
}
// only display 1 octant
void MyOctant::DisplayCurrent(vector3 a_v3Color)
{
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(IDENTITY_M4, m_v3Size), a_v3Color);
}
// display all octants
void Simplex::MyOctant::DisplayAll(vector3 a_v3Color)
{
	if (IsLeaf())
	{
		DisplayCurrent(a_v3Color);
	}
	else {
		for (uint i = 0; i < m_uChildren; ++i)
		{
			m_pChild[i]->DisplayAll(a_v3Color);
		}
	}
}
//clear all child nodes
void MyOctant::ClearEntityList(void)
{
	for (uint i = 0; i < m_uChildren; ++i)
	{
		m_pChild[i]->ClearEntityList();
	}
	m_lEntityList.clear();
}

void MyOctant::Subdivide(void)
{

	if (m_uLevel >= m_uMaxLevel || !ContainsMoreThan(m_uIdealEntityCount))
	{
		m_pRoot->m_lChildren.push_back(this);
		m_uLeafCount += 1;
		return;
	}


	if (m_uChildren == 8)
	{
		return;
	}

	//create each octant at updated position
	m_pChild[0] = new MyOctant(m_v3Center + vector3(-m_v3Size.x / 4, m_v3Size.y / 4, -m_v3Size.z / 4), m_v3Size / 2.0f);
	m_pChild[1] = new MyOctant(m_v3Center + vector3(-m_v3Size.x / 4, m_v3Size.y / 4, m_v3Size.z / 4), m_v3Size / 2.0f);
	m_pChild[2] = new MyOctant(m_v3Center + vector3(-m_v3Size.x / 4, -m_v3Size.y / 4, -m_v3Size.z / 4), m_v3Size / 2.0f);
	m_pChild[3] = new MyOctant(m_v3Center + vector3(-m_v3Size.x / 4, -m_v3Size.y / 4, m_v3Size.z / 4), m_v3Size / 2.0f);
	m_pChild[4] = new MyOctant(m_v3Center + vector3(m_v3Size.x / 4, -m_v3Size.y / 4, -m_v3Size.z / 4), m_v3Size / 2.0f);
	m_pChild[5] = new MyOctant(m_v3Center + vector3(m_v3Size.x / 4, -m_v3Size.y / 4, m_v3Size.z / 4), m_v3Size / 2.0f);
	m_pChild[6] = new MyOctant(m_v3Center + vector3(m_v3Size.x / 4, m_v3Size.y / 4, -m_v3Size.z / 4), m_v3Size / 2.0f);
	m_pChild[7] = new MyOctant(m_v3Center + vector3(m_v3Size.x / 4, m_v3Size.y / 4, m_v3Size.z / 4), m_v3Size / 2.0f);

	m_uChildren = 8;

	//initialize children
	for (uint i = 0; i < m_uChildren; ++i)
	{
		m_pChild[i]->m_pParent = this;
		m_pChild[i]->m_uLevel = m_uLevel + 1;
		m_pChild[i]->m_pRoot = m_pRoot;
		// add rigid body
		for (uint j = 0; j < m_uCurrEntityCount; ++j)
		{
			if (m_pChild[i]->IsColliding(m_lEntityList[j]))
				m_pChild[i]->m_lEntityList.push_back(m_lEntityList[j]);
		}

		//update entity count
		m_pChild[i]->m_uCurrEntityCount = m_pChild[i]->m_lEntityList.size();

		m_pChild[i]->Subdivide();
	}
}

//kills all nodes minus root
void MyOctant::KillBranches(void)
{
	if (IsLeaf())
	{
		return;
	}
	else
	{
		for (uint i = 0; i < m_uChildren; ++i)
		{
			m_pChild[i]->KillBranches();
			SafeDelete(m_pChild[i]);
		}
	}
}

//configure dimensions for leaves
void Simplex::MyOctant::ConfigureDimensions()
{
	if (IsLeaf())
	{
		for (uint i = 0; i < m_uCurrEntityCount; ++i)
		{
			m_pEntityMngr->AddDimension(m_lEntityList[i], m_uID);
		}
	}
	else
	{
		for (uint i = 0; i < m_uChildren; ++i)
		{
			m_pChild[i]->ConfigureDimensions();
		}
	}
}


void MyOctant::Release(void)
{
	if (this == m_pRoot)
	{
		KillBranches(); //deforestation 
	}
}

void MyOctant::Init(void)
{
	m_pEntityMngr = MyEntityManager::GetInstance();
	m_pMeshMngr = MeshManager::GetInstance();

	m_uID = m_uOctantCount;
	m_uOctantCount += 1;
}
