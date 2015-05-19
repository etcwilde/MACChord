#include "ChordNode.hpp"

ChordNode::ChordNode(const std::string& id) :
	m_id(id),
	m_key(id),
	m_predecessor(NULL),
	m_successor(NULL)
{
	create();
}

// TODO:
// Optimization would include not building a new key every time
// Create a different `findSuccessor(ChordKey k) const` function
ChordNode* ChordNode::findSuccessor(const std::string& id) const
{
	ChordKey key(id);
	if (this == m_successor)
	{
		std::cout << "Returning this\n";
		return (ChordNode*)this;
	}
	if (key.isBetween(m_key, m_successor->getKey()) || key == m_successor->getKey())
	{
		std::cout << "Returning successor\n";
		return m_successor;
	}
	else
	{
		std::cout << "Get closest preceding node\n";
		ChordNode* node = getClosesNode(key);
		if (node == this) return m_successor->findSuccessor(id);
		return node->findSuccessor(id);
	}
}

void ChordNode::create()
{
	m_predecessor = NULL;
	m_successor = this;
}

void ChordNode::join(const ChordNode& node)
{
	m_predecessor = NULL;
	std::cout << "Joining\n";
	m_successor = node.findSuccessor(getId());
	std::cout << "Found successor\n";
}

void ChordNode::stabilize()
{
	ChordNode* node = m_successor->getPredecessor();
	if (node != NULL)
	{
		ChordKey key = node->getKey();
		if ((this == m_successor) ||
				key.isBetween(this->getKey(), m_successor->getKey()))
			m_successor = node;
	}

	// Notify predecessors
	m_successor->notify(this);
}

void ChordNode::notify(ChordNode* node)
{
	const ChordKey key = node->getKey();
	if (m_predecessor == NULL ||
			key.isBetween(m_predecessor->getKey(), this->getKey()))
		m_predecessor = node;
}

ChordNode* ChordNode::getClosesNode(ChordKey key) const
{
	//TODO: Implement fingertable
	std::cout << "Finger table not implemented!\n";
	/*for (unsigned int i = 19; i >= 0; i--)
	{
		// Use finger table look up

	} */
	return (ChordNode*)this;
}