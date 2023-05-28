#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <fstream>

using namespace std;
using namespace chrono;

bool show=false;
static int times = 1;

enum PopulateType
{
    RandomHigherThanSize,
    RandomRange,
    NotDecreasing,
};

enum PivotType
{
    LastElement,
    RandomElement,
    Median3,
    Median5,
    Median7,
};

enum PartitionType{
    AlwaysSwap,
    SometimesSwap,
    NeverSwap,
};
void WriteOutArray(int* array, int size)
{
    if (show)
    {
        for(int i=0; i<size; i++)
        {
            cout<<array[i]<<" ";
        }
        cout<<" "<<endl;
    }
}

void Print(string message)
{
    if (show)
    {
        cout<< message;
    }
}


int* Populate(int size, PopulateType type)
{
    int* array = new int[size];

    switch (type)
    {
        case RandomHigherThanSize:
            for (int i = 0; i < size; i++)
            {
                array[i] = rand() % size + 1 + size;
            }
            break;
        case RandomRange:
            for (int i = 0; i < size; i++)
            {
                array[i] = rand() % 100;
            }
            break;
        case NotDecreasing:
            for (int i = 0; i < size; i++)
            {
                array[i] = i;
            }
            break;
        default:
            break;
    }
    return array;
}

void swap(int* array, int left, int right)
{
    int tmp = array[right];
    array[right] = array[left];
    array[left] = tmp;
}

int PivotMedian3(int* arr, int size, int left, int right)
{
    if(size<3)
    {
        throw invalid_argument("Array size too small");
    }

    int middleIndex = (left + right)/2;

    int tmp[3] =
    {
        arr[left],
        arr[middleIndex],
        arr[right]
    };

    sort(begin(tmp), end(tmp));

    arr[left] = tmp[0];
    arr[middleIndex] = tmp[1];
    arr[right] = tmp[2];

    swap(arr, middleIndex, right);

    return arr[right];
}

int PivotMedian5(int* arr, int size, int left, int right)
{
    if(size<5)
    {
        throw invalid_argument("Array size too small");
    }

    int middleIndex = (left + right)/2;

    int tmp[5] =
    {
        arr[left],
        arr[(left + middleIndex)/2],
        arr[middleIndex],
        arr[(middleIndex + right)/2],
        arr[right]
    };

    sort(begin(tmp), end(tmp));

    arr[left] = tmp[0];
    arr[(left + middleIndex)/2] = tmp[1];
    arr[middleIndex] = tmp[2];
    arr[(middleIndex + right)/2] = tmp[3];
    arr[right] = tmp[4];

    swap(arr, middleIndex, right);

    return arr[right];
}

int PivotMedian7(int* arr, int size, int left, int right)
{
    if(size<7)
    {
        throw invalid_argument("Array size too small");
    }

    int middleIndex = (left + right)/2;

    int tmp[7] =
    {
        arr[left],
        arr[left + (middleIndex - left)/3],
        arr[left + 2 * (middleIndex - left)/3],
        arr[middleIndex],
        arr[middleIndex + (right - middleIndex)/3],
        arr[middleIndex + 2 *(right - middleIndex)/3],
        arr[right]
    };

    sort(begin(tmp), end(tmp));

    arr[left] = tmp[0];
    arr[left + (middleIndex - left)/3] = tmp[1];
    arr[left + 2 *(middleIndex - left)/3] = tmp[2];
    arr[middleIndex] = tmp[3];
    arr[middleIndex + (right - middleIndex)/3] = tmp[4];
    arr[middleIndex + 2 * (right - middleIndex)/3] = tmp[5];
    arr[right] = tmp[6];

    swap(arr, middleIndex, right);

    return arr[right];
}

int PivotMedian(int* arr, int size, int left, int right, int n)
{
    int sum = 0;
    int count = 0;
    for(int i = left; i <= right; i += n)
    {
        sum += arr[i];
        count++;
    }
    int med = sum/count;
    int min= INT_MAX;
    int minIndex=0;

    for(int i=0; i < size; i++)
    {
        if(abs(arr[i] - med) <= min)
        {
            min = abs(arr[i] - med);
            minIndex = i;
        }
    }

    swap(arr, minIndex, right);

    return arr[right];
}

int Partition(int* arr, int size, int left, int right, PivotType type, PartitionType partType)
{
    int pivot;
    int index = rand()%(right - left) + left;

    switch(type)
    {
        case LastElement:
            pivot = arr[right];
            break;
        case RandomElement:
            if(index != right)
            {
                swap(arr, index, right);
            }
            pivot = arr[right];
            break;
        case Median3:
            pivot = PivotMedian3(arr, size, left, right);
            break;
        case Median5:
            pivot = PivotMedian5(arr, size, left, right);
            break;
        case Median7:
            pivot = PivotMedian7(arr, size, left, right);
            break;
        default:
            break;
    }

    int j = left-1;

    for(int i = left; i < right; i++)
    {
        if(arr[i] == pivot)
        {
            switch(partType)
            {
                case AlwaysSwap:
                    swap(arr, i, right);
                    break;
                case SometimesSwap:
                    if(rand()%2==0)
                    {
                        swap(arr, i, right);
                    }
                    break;
                case NeverSwap:
                    break;
                default:
                    break;
            }
        }
        else if(arr[i] < pivot)
        {
            j++;
            swap(arr, j, i);
        }
    }
    swap(arr, j + 1, right);
    return j + 1;
}

void Quick_Sort(int* array, int size, int leftIndex, int rightIndex, PivotType type, PartitionType partType)
{
    if (leftIndex >= rightIndex)
    {
        return;
    }

    int pivot = Partition(array, size, leftIndex, rightIndex, type, partType);
    Quick_Sort(array, size, leftIndex, pivot - 1, type, partType);
    Quick_Sort(array, size, pivot + 1, rightIndex, type, partType);
}


void RunForTime(PivotType type, PopulateType popType, PartitionType partType)
{
    int* array;

    time_point<system_clock> start,end;

    duration<double> dur= end - start;
    duration<double> durMini= end - start;

    cout << "TYP UZUPELNIENIA::: " << popType + 1 << endl;
    cout<<"i elementow---czas"<<endl;
    for (int i = 10 * 1000; i <= 100 * 1000 ; i += 10 * 1000)
    {
        for (int j = 0; j < times; j++)
        {
            array = Populate(i, popType);

            WriteOutArray(array,i);

            start = system_clock::now();

            Quick_Sort(array, i, 0, i - 1, type, partType);
            end = system_clock::now();

            WriteOutArray(array,i);

            delete[] array;

            durMini = end-start;
            dur+= end - start;
        }
        cout<<i<<"---"<<duration_cast<milliseconds>(durMini).count()<<endl;
    }

    cout << "CZAS LACZNY::: " << duration_cast<milliseconds>(dur).count()/times << endl;
    cout<<endl;
}
//uruchamia sortowanie dla wszystkich typow uzupelnienia tablicy
void RunAllPopulateTypes(PivotType type, PartitionType partTime)
{
    cout << "TYP PARTITION::: " << partTime + 1 << endl;
    RunForTime(type, RandomHigherThanSize, partTime);
    RunForTime(type, RandomRange, partTime);
    RunForTime(type, NotDecreasing, partTime);
    cout<<endl;
}
//uruchamia sortowanie dla jednego typu wybierania pivota, ale dla wszystkich rodzaju partycji
void Run( PivotType type)
{
    cout << "TYP PIVOTA::: " << type + 1 << endl;
    RunAllPopulateTypes(type, AlwaysSwap);
    RunAllPopulateTypes(type, SometimesSwap);
    RunAllPopulateTypes(type, NeverSwap);
    cout<<endl;
}

void RunAll()
{
    Run(LastElement);
    Run(RandomElement);
    Run(Median3);
    Run(Median5);
    Run(Median7);
}


 int main()
{
    srand(time(NULL));

    cout<<"TYPY PIVOTA:"<<endl;
    cout<<"1 - ostatni"<<endl;
    cout<<"2 - losowy"<<endl;
    cout<<"3 - mediana3"<<endl;
    cout<<"4 - mediana5"<<endl;
    cout<<"5 - mediana7"<<endl;
    cout<<endl;
    cout<<"TYPY PARTITION:"<<endl;
    cout<<"1 - zawsze zamieniaj"<<endl;
    cout<<"2 - czasami zamieniaj"<<endl;
    cout<<"3 - nie zamienaj"<<endl;
    cout<<endl;
    cout<<"TYPY UZUPELNIENIA TABELI:"<<endl;
    cout<<"1 - losowe wieksze niz rozmiar"<<endl;
    cout<<"2 - losowe zakres 0-99"<<endl;
    cout<<"3 - uporzadkowane"<<endl;
    cout<<endl;

    RunAll();

    return 0;
}
