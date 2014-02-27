#include "table.h"

#include <stdlib.h>
#include <string.h>
#include <algorithm>

#include "serial-util.h"
#include "exception.h"
#include "util.h"

/* Init normally */
Table::Table(int x, int y /*= 1*/, int z /*= 1*/)
	:m_x(x), m_y(y), m_z(z)
{
	data = static_cast<int16_t*>(calloc(x * y * z, sizeof(int16_t)));
}

Table::~Table()
{
	free(data);
}

int16_t Table::get(int x, int y, int z) const
{
	return data[m_x*m_y*z + m_x*y + x];
}

void Table::set(int16_t value, int x, int y, int z)
{
	if (x < 0 || x >= m_x
	||  y < 0 || y >= m_y
	||  z < 0 || z >= m_z)
	{
		return;
	}

	data[m_x*m_y*z + m_x*y + x] = value;

}

void Table::resize(int x, int y, int z)
{
	if (x == m_x && y == m_y && z == m_z)
		return;

	/* Fastpath: only z changed */
	if (x == m_x && y == m_y)
	{
		data = static_cast<int16_t*>(realloc(data, m_x * m_y * z * sizeof(int16_t)));
		int diff = z - m_z;
		if (diff > 0)
			memset(data + (m_x * m_y * m_z), 0, diff * m_x * m_y * sizeof(int16_t));
		goto done;
	}
	else
	{
		int16_t *newData = static_cast<int16_t*>(calloc(x * y * z, sizeof(int16_t)));

		for (int i = 0; i < std::min(x, m_x); ++i)
			for (int j = 0; j < std::min(y, m_y); ++j)
				for (int k = 0; k < std::min(z, m_z); k++)
				{
					int index = x*y*k + x*j + i;
					newData[index] = at(i, j, k);
				}

		free(data);
		data = newData;
	}

	done:
	m_x = x;
	m_y = y;
	m_z = z;

	return;
}

void Table::resize(int x, int y)
{
	if (x == m_x && y == m_y)
		return;

	/* Fastpath: treat table as two dimensional */
	if (m_z == 1)
	{
		/* Fastpath: only y changed */
		if (x == m_x)
		{
			data = static_cast<int16_t*>(realloc(data, m_x * y * sizeof(int16_t)));
			int diff = y - m_y;
			if (diff > 0)
				memset(data + (m_x * m_y), 0, diff * m_x * sizeof(int16_t));
			goto done;
		}
		else
		{
			int16_t *newData = static_cast<int16_t*>(calloc(x * y, sizeof(int16_t)));

			for (int i = 0; i < std::min(x, m_x); ++i)
				for (int j = 0; j < std::min(y, m_y); ++j)
				{
					int index = x*j + i;
					newData[index] = at(i, j);
				}

			free(data);
			data = newData;
		}

		done:
		m_x = x;
		m_y = y;

		return;
	}
	else
	{
		resize(x, y, m_z);
	}
}

void Table::resize(int x)
{
	if (x == m_x)
		return;

	/* Fastpath: treat table as one dimensional */
	if (m_y == 1 && m_z == 1)
	{
		data = static_cast<int16_t*>(realloc(data, x * sizeof(int16_t)));
		int diff = x - m_x;
		if (diff > 0)
			memset(data + (m_x), 0, diff * sizeof(int16_t));

		m_x = x;
		return;
	}

	resize(x, m_y);
}

/* Serializable */
int Table::serialSize() const
{
	/* header + data */
	return 20 + (m_x * m_y * m_z) * 2;
}

void Table::serialize(char *buffer) const
{
	char *buff_p = buffer;

	/* Table dimensions: we don't care
	 * about them but RMXP needs them */
	int dim = 1;

	if (m_y > 1)
		dim = 2;

	if (m_z > 1)
		dim = 3;

	write_int32(&buff_p, dim);
	write_int32(&buff_p, m_x);
	write_int32(&buff_p, m_y);
	write_int32(&buff_p, m_z);
	write_int32(&buff_p, m_x * m_y * m_z);

	for (int i = 0; i < m_z; ++i)
		for (int j = 0; j < m_y; ++j)
			for (int k = 0; k < m_x; k++)
				write_int16(&buff_p, at(k, j, i));
}


Table *Table::deserialize(const char *data, int len)
{
	if (len < 20)
		throw Exception(Exception::RGSSError, "Marshal: Table: bad file format");

	uint idx = 0;

	read_int32(data, idx);
	int x = read_int32(data, idx);
	int y = read_int32(data, idx);
	int z = read_int32(data, idx);
	int size = read_int32(data, idx);

	if (size != x*y*z)
		throw Exception(Exception::RGSSError, "Marshal: Table: bad file format");

	if (len != 20 + x*y*z*2)
		throw Exception(Exception::RGSSError, "Marshal: Table: bad file format");

	Table *t = new Table(x, y, z);

	for (int i = 0; i < z; ++i)
		for (int j = 0; j < y; ++j)
			for (int k = 0; k < x; k++)
				t->at(k, j, i) = read_int16(data, idx);

	return t;
}
