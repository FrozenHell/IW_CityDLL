// ArrayDLL.cpp : Defines the exported functions for the DLL application.

#include "stdafx.h"
#include <stdio.h>
#include <assert.h>
#include <Math.h>

template<typename DataType>
struct TArray
{
	DataType* Data;

	int Num()
	{
		return ArrayNum;
	}

	void Reallocate(int NewNum, bool bCompact=false)
	{
		ArrayNum = NewNum;
		if( ArrayNum > ArrayMax || bCompact )
		{
			ArrayMax = ArrayNum;
			Data = (DataType*)(*ReallocFunctionPtr)( Data, ArrayMax * sizeof(DataType), 8);
		}
	}
private:
	int ArrayNum;
	int ArrayMax;
};

struct NaviStruct
{
	TArray<int> NaviData;
};

int myrand_seed = 1021;
int myrand_rand()
{
	__asm
	{
		push	edx
		push	ecx
		mov		eax,	[myrand_seed]

		cmp		eax,	0
		jnz		noninit
		mov		[myrand_seed], 1021
		mov		eax,	[myrand_seed]

noninit:
		xor		edx,	edx
		mov		ecx,	127773
		div		ecx
		mov		ecx,	eax
		mov		eax,	16807
		mul		edx
		mov		edx,	ecx
		mov		ecx,	eax
		mov		eax,	2836
		mul		edx
		sub		ecx,	eax
		xor		edx,	edx
		mov		eax,	ecx
		mov		[myrand_seed], ecx
		mov		ecx,	100000
		div		ecx
		mov		eax,	edx
		pop		ecx
		pop		edx
	}
}

//---------------------------------------------------------------------------------------------------------

void MassRect(int** arr, int xMin, int xMax, int yMin, int yMax, int number)
{
	for (int i = xMin; i <= xMax; i++)
	{
		for (int j = yMin; j <= yMax; j++)
		{
			arr[i][j] = number;  
		}
	}
}

void MassNewHouse(int** arr, int number, int xSize, int ySize, int xPos, int yPos,
				  int xPlotsize, int yPlotsize)
{
	int xMin = 0, xMax = 0, yMin = 0, yMax = 0;

	if (xPos - xSize / 2 < 0)
	{
		xMin = 0;
		xMax = xSize - 1;
	}
	else if (xPos + xSize / 2 + xSize % 2 > xPlotsize)
	{
		xMin = xPlotsize - xSize;
		xMax = xPlotsize - 1;
	}
	else
	{
		xMin = xPos - xSize / 2;
		xMax = xPos + xSize / 2 + xSize % 2 - 1;
	}

	if (yPos - ySize/2 < 0)
	{
		yMin = 0;
		yMax = ySize - 1;
	}
	else if (yPos + ySize / 2 + ySize % 2 > yPlotsize)
	{
		yMin = yPlotsize - ySize;
		yMax = yPlotsize - 1;
	}
	else
	{
		yMin = yPos - ySize / 2;
		yMax = yPos + ySize / 2 + ySize % 2 - 1;
	}

	MassRect(arr, xMin, xMax, yMin, yMax, number);
}

bool MassInc(int** arr, int number, int xPlotsize, int yPlotsize)
{
	int minX = xPlotsize + 100, minY = yPlotsize + 100, maxX = -5, maxY = -5;

	for (int i = 0; i < xPlotsize; i++)
	{
		for (int j = 0; j < yPlotsize; j++)
		{
			if (arr[i][j] == number)
			{
				if (i < minX)
					minX = i;
				if (i > maxX)
					maxX = i;
				if (j < minY)
					minY = j;
				if (j > maxY)
					maxY = j;
			}
		}
	}

	// определяем случайное направление
	bool bIncX = myrand_rand() % 2 == 0 ? true : false;
	bool bIncF = myrand_rand() % 2 == 0 ? true : false;

	// ищем первое свободное направление, начиная с текущего
	for (int step = 0; step < 4; step++)
	{
		if (bIncX)
		{
			if (bIncF)
			{
				// лево
				if (minX != 0)
				{ // если мы не с левого края
					if (minX == 1)
					{ // если мы на второй линии
						bool bfree = true;
						for (int i = max(minY - 1, 0); i <= min(maxY + 1, yPlotsize - 1); i++)
						{ // проверяем, свободна ли первая линия
							if (arr[0][i] != -1)
								bfree = false;
						}

						if (bfree)
						{ // если есть куда расти
							// увеличиваем здание влево
							for (int i = minY; i <= maxY; i++)
								arr[minX - 1][i] = number;

							// выходим из функции
							return true;
						}
					}
					else
					{ // если мы на третьей+ линии
						bool bfree = true;
						for (int i = max(minY - 1, 0); i <= min(maxY + 1, yPlotsize - 1); i++)
						{ // проверяем, свободны ли две предыдущие линии
							if (arr[minX - 1][i] != -1)
								bfree = false;
							if (arr[minX - 2][i] != -1)
								bfree = false;
						}

						if (bfree)
						{ // если есть куда расти
							// увеличиваем здание влево
							for (int i = minY; i <= maxY; i++)
								arr[minX - 1][i] = number;

							// выходим из функции
							return true;
						}
					}
				}
			}
			else
			{
				// право
				if (maxX != xPlotsize - 1)
				{ // если мы не с правого края
					if (maxX == xPlotsize - 2)
					{ // если мы на второй линии
						bool bfree = true;
						for (int i = max(minY - 1, 0); i <= min(maxY + 1, yPlotsize - 1); i++)
						{ // проверяем, свободна ли первая линия
							if (arr[xPlotsize - 1][i] != -1)
								bfree = false;
						}

						if (bfree)
						{ // если есть куда расти
							// увеличиваем здание вправо
							for (int i = minY; i <= maxY; i++)
								arr[maxX + 1][i] = number;

							// выходим из функции
							return true;
						}
					}
					else
					{ // если мы на третьей+ линии
						bool bfree = true;
						for (int i = max(minY - 1, 0); i <= min(maxY + 1, yPlotsize - 1); i++)
						{ // проверяем, свободны ли две предыдущие линии
							if (arr[maxX + 1][i] != -1)
								bfree = false;
							if (arr[maxX + 2][i] != -1)
								bfree = false;
						}

						if (bfree)
						{ // если есть куда расти
							// увеличиваем здание вправо
							for (int i = minY; i <= maxY; i++)
								arr[maxX + 1][i] = number;

							// выходим из функции
							return true;
						}
					}
				}
			}
		}
		else
		{
			if (bIncF)
			{
				// верх
				if (minY != 0)
				{ // если мы не с левого края
					if (minY == 1)
					{ // если мы на второй линии
						bool bfree = true;
						for (int i = max(minX - 1, 0); i <= min(maxX + 1, xPlotsize - 1); i++)
						{ // проверяем, свободна ли первая линия
							if (arr[i][0] != -1)
								bfree = false;
						}

						if (bfree)
						{ // если есть куда расти
							// увеличиваем здание влево
							for (int i = minX; i <= maxX; i++)
								arr[i][minY - 1] = number;

							// выходим из функции
							return true;
						}
					}
					else
					{ // если мы на третьей+ линии
						bool bfree = true;
						for (int i = max(minX - 1, 0); i <= min(maxX + 1, xPlotsize - 1); i++)
						{ // проверяем, свободны ли две предыдущие линии
							if (arr[i][minY - 1] != -1)
								bfree = false;
							if (arr[i][minY - 2] != -1)
								bfree = false;
						}

						if (bfree)
						{ // если есть куда расти
							// увеличиваем здание влево
							for (int i = minX; i <= maxX; i++)
								arr[i][minY - 1] = number;

							// выходим из функции
							return true;
						}
					}
				}
			}
			else
			{
				// низ
				if (maxY != yPlotsize - 1)
				{ // если мы не с правого края
					if (maxY == yPlotsize - 2)
					{ // если мы на второй линии
						bool bfree = true;
						for (int i = max(minX - 1, 0); i <= min(maxX + 1, xPlotsize - 1); i++)
						{ // проверяем, свободна ли первая линия
							if (arr[i][yPlotsize - 1] != -1)
								bfree = false;
						}

						if (bfree)
						{ // если есть куда расти
							// увеличиваем здание вправо
							for (int i = minX; i <= maxX; i++)
								arr[i][maxY + 1] = number;

							// выходим из функции
							return true;
						}
					}
					else
					{ // если мы на третьей+ линии
						bool bfree = true;
						for (int i = max(minX - 1, 0); i <= min(maxX + 1, xPlotsize - 1); i++)
						{ // проверяем, свободны ли две предыдущие линии
							if (arr[i][maxY + 1] != -1)
								bfree = false;
							if (arr[i][maxY + 2] != -1)
								bfree = false;
						}

						if (bfree)
						{ // если есть куда расти
							// увеличиваем здание вправо
							for (int i = minX; i <= maxX; i++)
								arr[i][maxY + 1] = number;

							// выходим из функции
							return true;
						}
					}
				}
			}
		}

		bIncF = !bIncF;
		if (step == 1)
		{
			bIncX = !bIncX;
		}
	}

	// не смогли увеличить здание ни в одну сторону
	return false;
}

class House
{
public:
	// положение и размер дома
	int X, Y;
	int Len, Wid, Hei;

	// полностью завершено
	bool bIsCompleted;

	// создаём новый дом
	void NewHouse(int xPos, int yPos, int xSize, int ySize, int zSize)
	{
		X = xPos;
		Y = yPos;
		Len = xSize;
		Wid = ySize;
		Hei = zSize;
	}

	// уничтожение дома
	void Destroy()
	{
		bIsCompleted = false;
	}
};

class Cluster
{
public:
	// положение и размер кластера
	int X, Y;
	int Len, Wid;

	// здания в кластере
	House* Houses;
	int HousesCount;

	// создать кластер
	// houseSize - минимальный размер дома в кластере
	void Generate(int xPos, int yPos, int xSize, int ySize, int houseSize)
	{
		X = xPos;
		Y = yPos;
		Len = xSize;
		Wid = ySize;

		// максимальное количество домов, которыми можно застроить эту область
		int xMaxCount = min((Len + 1) / (houseSize + 1), 3);
		int yMaxCount = min((Wid + 1) / (houseSize + 1), 3);
		int MaxCount = xMaxCount * yMaxCount;

		// выбираем количество домов
		HousesCount = MaxCount/3 + 1 + myrand_rand()%(MaxCount - MaxCount/3);

		if (HousesCount > 8)
			HousesCount = 8;

		//ShowMessage(IntToStr(HousesCount));

		Houses = new House[HousesCount];

		for (int i = 0; i < HousesCount; i++)
			Houses[i].bIsCompleted = false;

		// временный массив для определение площади покрытия домов
		int** hoArr = new int*[Len];
		for(int i = 0; i<Len;i++)
			hoArr[i] = new int[Wid];

		// очищаем массив
		for (int i = 0; i < Len; i++)
			for (int j = 0; j < Wid; j++)
				hoArr[i][j] = -1;

		// первое здание
		MassNewHouse(hoArr, 0, houseSize, houseSize, 0, 0, Len, Wid);

		// второе здание
		if (HousesCount > 1)
			MassNewHouse(hoArr, 1, houseSize, houseSize, Len-1, Wid-1, Len, Wid);

		// третье и четвёртое здание
		if (HousesCount > 2)
		{
			if (xMaxCount > 1 && yMaxCount > 1)
			{
				int leftOrRight = myrand_rand()%2;

				if (leftOrRight == 0)
					MassNewHouse(hoArr, 2, houseSize, houseSize, 0, Wid-1, Len, Wid);
				else
					MassNewHouse(hoArr, 2, houseSize, houseSize, Len-1, 0, Len, Wid);

				if (HousesCount > 3)
					if (leftOrRight == 0)
						MassNewHouse(hoArr, 3, houseSize, houseSize, Len-1, 0, Len, Wid);
					else
						MassNewHouse(hoArr, 3, houseSize, houseSize, 0, Wid-1, Len, Wid);
			}
			else
			{
				// если в углы не получится вставлять блоки, ставим блок по центру
				MassNewHouse(hoArr, 2, houseSize, houseSize, Len/2, Wid/2, Len, Wid);
			}
		}

		// 5-8 здания
		if (HousesCount > 4)
		{
			// определяем случайное направление
			bool bIncX;
			bool bIncF = myrand_rand()%2 == 0 ? true : false;

			if (Len < Wid)
				bIncX = true;
			else if (Len > Wid)
				bIncX = false;
			else
				bIncX = myrand_rand()%2 == 0 ? true : false;

			for (int step = 4; step < HousesCount; step++)
			{
				if (bIncX)
				{
					if (bIncF)
					{
						// лево
						MassNewHouse(hoArr, step, houseSize, houseSize, 0, Wid/2, Len, Wid);
					}
					else
					{
						// право
						MassNewHouse(hoArr, step, houseSize, houseSize, Len, Wid/2, Len, Wid);
					}
				}
				else
				{
					if (bIncF)
					{
						// верх
						MassNewHouse(hoArr, step, houseSize, houseSize, Len/2, 0, Len, Wid);
					}
					else
					{
						// низ
						MassNewHouse(hoArr, step, houseSize, houseSize, Len/2, Wid, Len, Wid);
					}
				}

				bIncF = !bIncF;
				if (step == 5)
					bIncX = !bIncX;
			}
		}

		int completedBuildings = 0;
		while (completedBuildings < HousesCount)
		{
			for (int i = 0; i < HousesCount; i++)
			{
				if (!Houses[i].bIsCompleted)
				{
					if (!MassInc(hoArr, i, Len, Wid))
					{
						Houses[i].bIsCompleted = true;
						completedBuildings++;
					}
				}
			}
		}

		for (int k = 0; k < HousesCount; k++)
		{
			int minX = Len + 100, minY = Wid + 100, maxX = -5, maxY = -5;

			for (int i = 0; i < Len; i++)
				for (int j = 0; j < Wid; j++)
				{
					if (hoArr[i][j] == k)
					{
						if (i < minX)
							minX = i;
						if (i > maxX)
							maxX = i;
						if (j < minY)
							minY = j;
						if (j > maxY)
							maxY = j;
					}
				}

				int zSize = 3 + int(floor((houseSize * houseSize * houseSize * 2)/((double)(maxX - minX + 1) * (maxY - minY + 1))));
				Houses[k].NewHouse(X + minX, Y + minY, maxX - minX + 1, maxY - minY + 1, zSize);
		}

		// очистка памяти
		for (int i = 0; i<Len; i++)
			delete[] hoArr[i];
		delete[] hoArr;
	}

	// уничтожение кластера
	void Destroy()
	{
		for (int i = 0; i < HousesCount; i++)
		{
			Houses[i].Destroy();
		}
		delete[] Houses;
	}
};

class City
{
public:
	Cluster* Clusters;
	int ClustersCount;
	bool bIsGenerated;

	int GetClustersCount()
	{
		return ClustersCount;
	}

	void Generate(int count)
	{
		ClustersCount = count;
		Clusters = new Cluster[ClustersCount];
		int nextX1 = 1, sizeX1 = 0, nextY1 = 0, sizeY1 = 9 + myrand_rand() % 5;
		int nextX2 = -1, sizeX2 = 0, nextY2 = 0, sizeY2 = 9 + myrand_rand() % 5;
		int maxX = count/3;
		for (int i = 0; i < ClustersCount; i += 2) // i всегда кратно двум
		{
			sizeX1 = 9 + myrand_rand() % 5;
			Clusters[i].Generate(nextX1, nextY1, sizeX1, sizeY1, 5);
			nextX1 += sizeX1 + 2;

			sizeX2 = 9 + myrand_rand() % 5;
			Clusters[i + 1].Generate(nextX2 - sizeX2, nextY2, sizeX2, sizeY2, 5);
			nextX2 -= sizeX2 + 2;

			if (nextX1 > maxX)
			{
				nextX1 = 1;
				nextY1 += sizeY1 + 2;
				sizeY1 = 9 + myrand_rand() % 5;
			}

			if (nextX2 < -maxX)
			{
				nextX2 = -1;
				nextY2 += sizeY2 + 2;
				sizeY2 = 9 + myrand_rand() % 5;
			}    
		}
		bIsGenerated = true;
	}

	// уничтожение кластера
	void Destroy()
	{
		bIsGenerated = false;
		for (int i = 0; i < ClustersCount; i++)
		{
			Clusters[i].Destroy();
		}
		delete[] Clusters;
	}
};

// заполняем передаваемые данные
//Первый байт (n) отвечает за количество кластеров.
//Следующие n*5 байт отвечают за кластеры
//В последующих хранится информация о зданиях
void GetCityData(int* Data, City C)
{
	// узнаём количество кластеров
	int clCount = C.GetClustersCount();
	// записываем количество кластеров как первый элемент выходного массива
	Data[0] = clCount;
	
	// заполняем выходной массив
	int thisCluster = 0;
	for (int i = 0; i < clCount; i++)
	{
		// заполняем информацию для кластера
		Data[1 + i * 5]		= thisCluster + C.Clusters[i].HousesCount;
		Data[1 + i * 5 + 1] = C.Clusters[i].Len;
		Data[1 + i * 5 + 2] = C.Clusters[i].Wid;
		Data[1 + i * 5 + 3] = C.Clusters[i].X;
		Data[1 + i * 5 + 4] = C.Clusters[i].Y;
		for	(int j = thisCluster; j < thisCluster + C.Clusters[i].HousesCount; j++)
		{
			// заполняем информацию для здания
			Data[1 + clCount * 5 + j * 5]	  = C.Clusters[i].Houses[j - thisCluster].Len;
			Data[1 + clCount * 5 + j * 5 + 1] = C.Clusters[i].Houses[j - thisCluster].Wid;
			Data[1 + clCount * 5 + j * 5 + 2] = C.Clusters[i].Houses[j - thisCluster].Hei;
			Data[1 + clCount * 5 + j * 5 + 3] = C.Clusters[i].Houses[j - thisCluster].X;
			Data[1 + clCount * 5 + j * 5 + 4] = C.Clusters[i].Houses[j - thisCluster].Y;
		}
		// сдвиг информации о положении данных в массиве
		thisCluster += C.Clusters[i].HousesCount;
	}
}

// получаем информацию о размере памяти, которую надо выделить
int GetDataSize(City C)
{
	int z = 1;
	for (int i = 0; i < C.ClustersCount; i++)
	{
		z += 5 + 5 * C.Clusters[i].HousesCount;
	}
	return z;
}

//---------------------------------------------------------------------------------------------------------

extern "C"
{
	typedef void* (*ReallocFunctionPtrType)(void* Original, DWORD Count, DWORD Alignment);

	ReallocFunctionPtrType ReallocFunctionPtr = NULL;

	struct FDLLBindInitData
	{
		INT Version;
		ReallocFunctionPtrType ReallocFunctionPtr;
	};

	__declspec(dllexport) void DLLBindInit(FDLLBindInitData* InitData)
	{
		ReallocFunctionPtr = InitData->ReallocFunctionPtr;
	}

	__declspec(dllexport) void GetNavData(struct NaviStruct* NavData, int count, int seed)
	{
		myrand_seed = seed;
		City C1;
		C1.Generate(count);

		NavData->NaviData.Reallocate(GetDataSize(C1));

		GetCityData(NavData->NaviData.Data, C1);

		C1.Destroy();
	}
}
