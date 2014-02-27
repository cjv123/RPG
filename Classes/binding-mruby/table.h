#ifndef TABLE_H
#define TABLE_H

#include "serializable.h"

#include <stdint.h>

class Table : public Serializable
{
public:
	Table(int x, int y = 1, int z = 1);
	virtual ~Table();

	int xSize() const { return m_x; }
	int ySize() const { return m_y; }
	int zSize() const { return m_z; }

	int16_t get(int x, int y = 0, int z = 0) const;
	void set(int16_t value, int x, int y = 0, int z = 0);

	inline int16_t &at(int x, int y = 0, int z = 0)
	{
		int16_t &value = data[m_x*m_y*z + m_x*y + x];

		return value;
	}

	inline int16_t &at(int x, int y = 0, int z = 0) const
	{
		int16_t &value = data[m_x*m_y*z + m_x*y + x];

		return value;
	}

	void resize(int x, int y, int z);
	void resize(int x, int y);
	void resize(int x);

	int serialSize() const;
	void serialize(char *buffer) const;
	static Table *deserialize(const char *data, int len);

private:
	int m_x, m_y, m_z;

	int16_t *data;
};

#endif // TABLE_H
