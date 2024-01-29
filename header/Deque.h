#ifndef DEQUE_H_
#define DEQUE_H_

#include <assert.h>

typedef unsigned int ui;
typedef long long ll;

template<class T>
class Deque{
    public:
        struct iterator{
            public:
                Deque<T>ReservedDeque; // Declare ReservedDeque variable
                iterator():ptr(ReservedDeque){}
                iterator(const iterator& it):data(it.data), ptr(it.ptr){}
                iterator(iterator&& it):data(it.data), ptr(it.ptr){it.data=nullptr;}
                iterator(T* data, Deque<T>& ptr):data(data), ptr(ptr){}
                iterator& operator++(){++data;return *this;}
                iterator& operator--(){--data;return *this;}
                iterator operator++(int){iterator tmp=*this;++data;return tmp;}
                iterator operator--(int){iterator tmp=*this;--data;return tmp;}
                iterator operator+(const ll num){return iterator(data+(data-ptr.Data+num)%ptr.Capacity, ptr);}
                iterator operator-(const ll num){return iterator(data+(data-ptr.Data-num+ptr.Capacity)%ptr.Capacity, ptr);}
                iterator& operator+=(const ll num){data=(data+num)%ptr.Capacity;return *this;}
                iterator& operator-=(const ll num){data=(data-num+ptr.Capacity)%ptr.Capacity;return *this;}
                iterator& operator=(const iterator& it){data=it.data;return *this;}
                iterator& operator=(iterator&& it){data=it.data;it.data=nullptr;return *this;}
                bool operator==(const iterator& it)const{return data==it.data;}
                bool operator!=(const iterator& it)const{return data!=it.data;}
                bool operator<(const iterator& it)const{return data<it.data;}
                bool operator>(const iterator& it)const{return data>it.data;}
                bool operator<=(const iterator& it)const{return data<=it.data;}
                bool operator>=(const iterator& it)const{return data>=it.data;}
                T& operator*(){return *data;}
                const T& operator*()const{return *data;}
                T* operator->(){return data;}
                const T* operator->()const{return data;}
                T& operator*(const ui num){return *(data+num)%ptr.Capacity;}
                const T& operator*(const ui num)const{return *(data+num)%ptr.Capacity;}
                T& operator[](const ui num){return *(data+num)%ptr.Capacity;}
                const T& operator[](const ui num)const{return *(data+num)%ptr.Capacity;}

            private:
                Deque<T>& ptr;
                T* data;
        };
        friend struct iterator;
        
        struct reverseIterator{
            reverseIterator()=default;
            reverseIterator(const Deque<T>::iterator& it):rIt(it){}
            reverseIterator(const reverseIterator& it):rIt(it.rIt){}
            reverseIterator(reverseIterator&& it):rIt(it.rIt){it.rIt=nullptr;}
            reverseIterator& operator=(const Deque<T>::iterator& it){rIt=it;return *this;}
            reverseIterator& operator=(const reverseIterator& it){rIt=it.rIt;return *this;}
            reverseIterator& operator=(reverseIterator&& it){rIt=it.rIt,it.rIt=nullptr;return *this;}
            reverseIterator& operator++(){--rIt;return *this;}
            reverseIterator operator++(int){auto tmp=*this;--rIt;return tmp;}
            reverseIterator& operator--(){++rIt;return *this;}
            reverseIterator operator--(int){auto tmp=*this;++rIt;return tmp;}
            reverseIterator operator+(const long long num){return rIt-num;}
            reverseIterator operator-(const long long num){return rIt+num;}
            reverseIterator& operator+=(const long long num){rIt-=num;return *this;}
            reverseIterator& operator-=(const long long num){rIt+=num;return *this;}
            bool operator==(const reverseIterator& it)const{return rIt==it.rIt;}
            bool operator!=(const reverseIterator& it)const{return rIt!=it.rIt;}
            bool operator<(const reverseIterator& it)const{return rIt>it.rIt;}
            bool operator>(const reverseIterator& it)const{return rIt<it.rIt;}
            bool operator<=(const reverseIterator& it)const{return rIt>=it.rIt;}
            bool operator>=(const reverseIterator& it)const{return rIt<=it.rIt;}
            T* operator->(){return rIt.operator->();}
            const T* operator->()const{return rIt.operator->();}
            T& operator*(){return *rIt;}
            const T& operator*()const{return *rIt;}
            T& operator[](const ui num){return *(rIt-num);}
            const T& operator[](const ui num)const{return *(rIt-num);}

            Deque<T>::iterator rIt;
        };

    public:
        Deque();
        ~Deque();
        void PushFront(const T& data);
        void PushBack(const T& data);
        void PopFront();
        void PopBack();
        void Extend();
        T Front();
        T Back();
        const T Front()const;
        const T Back()const;
        bool Empty()const;
        ui Size()const;
        void Clear();
        Deque& operator=(const Deque& q);
        Deque& operator=(Deque&& q);
        T& operator[](const ui pos);
        const T& operator[](const ui pos)const;

        iterator Insert(const ui pos, const T& val);
        iterator Insert(iterator p, const T& val);
        iterator Erase(iterator p);
        iterator Erase(const ui pos, const ui size=1);
        
        const iterator cbegin()const;
        const iterator cend()const;
        const iterator begin()const;
        const iterator end()const;
        iterator begin();
        iterator end();

    private:
        T* Data;
        ui Begin;
        ui End;
        ui Capacity;
};

template<class T>
Deque<T>::Deque(){
    Capacity=2;
    Data=new T[Capacity];
    Begin=0;
    End=0;
}

template<class T>
Deque<T>::~Deque(){
    assert(Data!=nullptr);
    delete[] Data;
}

template<class T>
void Deque<T>::PushFront(const T& data){
    if(Size()==Capacity)Extend();
    Begin=(Begin-1+Capacity)%Capacity;
    Data[Begin]=data;
}

template<class T>
void Deque<T>::PushBack(const T& data){
    if(Size()==Capacity)Extend();
    Data[End]=data;
    End=(End+1)%Capacity;
}

template<class T>
void Deque<T>::PopFront(){
    assert(Begin!=End);
    Begin=(Begin+1)%Capacity;
}

template<class T>
void Deque<T>::PopBack(){
    assert(Begin!=End);
    End=(End-1+Capacity)%Capacity;
}

template<class T>
void Deque<T>::Extend(){
    Capacity*=2;
    T* temp=new T[Capacity];
    for(ui i=Begin, j=0;i!=End;i=(i+1)%Capacity, j++)
        temp[j]=Data[i];
    Begin=0;
    End=Size();
    delete[] Data;
    Data=temp;
}

template<class T>
T Deque<T>::Front(){
    assert(Begin!=End);
    return Data[Begin];
}

template<class T>
T Deque<T>::Back(){
    assert(Begin!=End);
    return Data[(End-1+Capacity)%Capacity];
}

template<class T>
const T Deque<T>::Front()const{
    assert(Begin!=End);
    return Data[Begin];
}

template<class T>
const T Deque<T>::Back()const{
    assert(Begin!=End);
    return Data[(End-1+Capacity)%Capacity];
}

template<class T>
bool Deque<T>::Empty()const{
    return Begin==End;
}

template<class T>
ui Deque<T>::Size()const{
    return (End-Begin+Capacity)%Capacity;
}

template<class T>
void Deque<T>::Clear(){
    Begin=0;
    End=0;
}

template<class T>
Deque<T>& Deque<T>::operator=(const Deque& q){
    if(this==&q)return *this;
    delete[] Data;
    Capacity=q.Capacity;
    Data=new T[Capacity];
    Begin=q.Begin;
    End=q.End;
    for(ui i=Begin, j=0;i!=End;i=(i+1)%Capacity, j++)
        Data[j]=q.Data[i];
    return *this;
}

template<class T>
Deque<T>& Deque<T>::operator=(Deque&& q){
    if(this==&q)return *this;
    delete[] Data;
    Capacity=q.Capacity;
    Data=q.Data;q.Data=nullptr;
    Begin=q.Begin;q.Begin=0;
    End=q.End;q.End=0;
    return *this;
}

template<class T>
T& Deque<T>::operator[](const ui pos){
    assert(pos<Size());
    return Data[(Begin+pos)%Capacity];
}

template<class T>
const T& Deque<T>::operator[](const ui pos)const{
    assert(pos<Size());
    return Data[(Begin+pos)%Capacity];
}

template<class T>
typename Deque<T>::iterator Deque<T>::Insert(const ui pos, const T& val){
    assert(pos<=Size());
    if(Size()==Capacity)Extend();
    ui i=End;
    for(;i!=Begin&&i!=pos;i=(i-1+Capacity)%Capacity)
        Data[i]=Data[(i-1+Capacity)%Capacity];
    Data[i]=val;
    End=(End+1)%Capacity;
    return iterator(Data+i, *this);
}

template<class T>
typename Deque<T>::iterator Deque<T>::Insert(iterator p, const T& val){
    assert(p>=Data&&p<Data+End&&p>=Data+Begin&&p<Data+Capacity);
    if(Size()==Capacity)Extend();
    ui i=End;
    for(;i!=Begin&&Data+i!=p;i=(i-1+Capacity)%Capacity)
        Data[i]=Data[(i-1+Capacity)%Capacity];
    Data[i]=val;
    End=(End+1)%Capacity;
    return iterator(Data+i, *this);
}

template<class T>
typename Deque<T>::iterator Deque<T>::Erase(iterator p){
    iterator tmp(Data, *this);
    assert(p>=tmp&&p<tmp+End&&p>=tmp+Begin&&p<tmp+Capacity);
    ui i=Begin;
    for(;i!=End;i=(i+1)%Capacity)
        Data[i]=Data[(i+1)%Capacity];
    End=(End-1+Capacity)%Capacity;
    return iterator(Data+i, *this);
}

template<class T>
typename Deque<T>::iterator Deque<T>::Erase(const ui pos, const ui size){
    assert(pos+size<=Size());
    ui i=pos;
    for(;i+size!=End;i=(i+1)%Capacity)
        Data[i]=Data[i+size];
    End=(End-size+Capacity)%Capacity;
    return iterator(Data+i, *this);
}

template<class T>
const typename Deque<T>::iterator Deque<T>::cbegin()const{
    return iterator(Data+Begin, *this);
}

template<class T>
const typename Deque<T>::iterator Deque<T>::cend()const{
    return iterator(Data+End, *this);
}

template<class T>
const typename Deque<T>::iterator Deque<T>::begin()const{
    return iterator(Data+Begin, *this);
}

template<class T>
const typename Deque<T>::iterator Deque<T>::end()const{
    return iterator(Data+End, *this);
}

template<class T>
typename Deque<T>::iterator Deque<T>::begin(){
    return iterator(Data+Begin, *this);
}

template<class T>
typename Deque<T>::iterator Deque<T>::end(){
    return iterator(Data+End, *this);
}

template<class T>
bool operator==(const Deque<T>& q1, const Deque<T>& q2){
    if(q1.Size()!=q2.Size())return false;
    for(ui i=0;i<q1.Size();i++)
        if(q1.Data[(q1.Begin+i)%q1.Capacity]!=q2.Data[(q2.Begin+i)%q2.Capacity])
            return false;
    return true;
}

template<class T>
bool operator!=(const Deque<T>& q1, const Deque<T>& q2){
    return !(q1==q2);
}

#endif