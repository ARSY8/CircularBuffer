#include "CircularBuffer.hpp"
#include <string.h>
#include <iostream>


CircularBuffer::CircularBuffer() : size_(0), capacity_(10)
{
	buffer = new value_type[capacity_];
}

CircularBuffer::~CircularBuffer() {
	delete[] buffer;
}

CircularBuffer::CircularBuffer(const CircularBuffer& cb) : size_(cb.size_), capacity_(cb.capacity_), head(cb.head), tail(cb.tail)
{
	buffer = new value_type[capacity_];
	memcpy(buffer, cb.buffer, capacity_ * sizeof(value_type));
}

CircularBuffer::CircularBuffer(int capacity) : capacity_(capacity)
{
	if (capacity_ < 0) {
		throw std::invalid_argument("Число не может быть отрицательным!");
	}
	//проверку на отрицательное значение
	buffer = new value_type[capacity];
}

CircularBuffer::CircularBuffer(int capacity, const value_type& elem) : CircularBuffer(capacity) {
	if (capacity_ < 0) {
		throw std::invalid_argument("Число не может быть отрицательным!");
	}
	for (int i = 0; i < capacity; i++) {
		push_front(elem);
	}
}

CircularBuffer& CircularBuffer::operator=(const CircularBuffer& cb) {
	if (this == &cb) return *this;

	size_ = cb.size_;
	capacity_ = cb.capacity_;
	head = cb.head;
	tail = cb.tail;

	delete[] buffer;
	buffer = new value_type[capacity_];
	memcpy(buffer, cb.buffer, capacity_	* sizeof(value_type));
	return *this;
}

value_type& CircularBuffer::operator[](int i) {
	int real_pos = tail - i;
	if (real_pos < 0)
		real_pos += capacity_;
	return buffer[real_pos];
}

const value_type& CircularBuffer::operator[](int i) const{
	int real_pos = tail - i;
	if (real_pos < 0)
		real_pos += capacity_;
	return buffer[real_pos];
}

void CircularBuffer::resize(int new_size, const value_type& item) {
	if (new_size < 0) 
		throw std::invalid_argument("Число не может быть отрицательным!");

	if (size_ == new_size) 
		return;

	linearize();

	if (new_size > size_) {
		for (int i = new_size - size_; i > 0; --i) {
			push_front(item);
		}
	}
	else {
		for(int i = size_ - new_size; i > 0; --i){
			pop_back();
		}
	}
}

value_type& CircularBuffer::at(int i) {
	if (i < size_ && i >= 0) {
		int real_pos = tail - i;
		if (real_pos < 0)
			real_pos += capacity_;
		return buffer[real_pos];
	}
	else {
		throw std::out_of_range("Индекс выходит за границы кольцевого буффера!");
	}
}

const value_type& CircularBuffer::at(int i) const {
	if (i < size_ && i >= 0) {
		int real_pos = tail - i;
		if (real_pos < 0)
			real_pos += capacity_;
		return buffer[real_pos];
	}
	else {
		throw std::out_of_range("Индекс выходит за границы кольцевого буффера!");
	}
}

value_type& CircularBuffer::front() {
	return buffer[head];
}

value_type& CircularBuffer::back() {
	return buffer[tail];
}

const value_type& CircularBuffer::front() const {
	return buffer[head];
}

const value_type& CircularBuffer::back() const {
	return buffer[tail];
}

value_type* CircularBuffer::linearize() {
	if (head < tail) {
		memcpy(buffer, buffer + head, size_ * sizeof(value_type));
	}
	else {
		int j;
		int p;
		for (int i = head; i > 0; --i) {
			j = i - 1;
			while (j != capacity_ - 1) {
				p = j + 1;
				std::swap(buffer[j], buffer[p]);
				j++;
			}
		}
	}
	head = 0;
	tail = size_ - 1;
	if (tail >= capacity_) tail = 0;
	if (head == tail) head++;
	return buffer;
}

bool CircularBuffer::is_linearized() const {
	return head == 0;
}

void CircularBuffer::rotate(int new_begin) {
	int index_new_begin = tail - new_begin;
	if (index_new_begin < 0)
		index_new_begin += capacity_;

	if (index_new_begin == 0) return;
	if (index_new_begin == head) linearize();

	int j;
	int p;
	for (int i = index_new_begin; i > 0; --i) {
		j = i - 1;
		while (j != capacity_ - 1) {
			p = j + 1;
			std::swap(buffer[j], buffer[p]);
			j++;
		}
	}
	tail = size_ - new_begin - 1;
	if(tail < 0)
		tail += capacity_; 
	head = capacity_ - new_begin;
	if(head < 0)
		head += capacity_;
}

int CircularBuffer::size() const {
	return size_;
}

bool CircularBuffer::empty() const {
	return size_ == 0;
}

bool CircularBuffer::full() const {
	return size_ == capacity_;
}

int CircularBuffer::reserve() const{
	return capacity_ - size_;
}

int CircularBuffer::capacity() const{
	return capacity_;
}

void CircularBuffer::swap(CircularBuffer& cb) {
	std::swap(cb.buffer, buffer);
	std::swap(cb.size_, size_);
	std::swap(cb.capacity_, capacity_);
	std::swap(cb.head, head);
	std::swap(cb.tail, tail);
}

void CircularBuffer::set_capacity(int new_capacity) {
	if (new_capacity > capacity_) {
		value_type* new_buffer = new value_type[new_capacity];
		memcpy(new_buffer, buffer, capacity_ * sizeof(value_type));

		delete[] buffer;
		buffer = new_buffer;
		capacity_ = new_capacity;
	}
	else {
		if (size_ > new_capacity) {
			linearize();
			tail = new_capacity - 1;
			size_ = new_capacity;
			head = 0;
		}
		capacity_ = new_capacity;
	}
}

void CircularBuffer::insert(int pos, const value_type& item) {
	if (pos < 0 || pos > size_ + 1) {
		throw std::overflow_error("Выход за пределы допустимого диапазона!");
	}

	int real_pos = tail - pos;
	if (real_pos < 0)
		real_pos += capacity_;
	buffer[real_pos] = item;
}

void CircularBuffer::erase(int first, int last) {
	if (first < 0 || last < 0 || last < first) 
		throw std::overflow_error("Выход за пределы допустимого диапазона!");

	if (first == 1 && last == size_) clear();
	if (first != 1 && last == size_) {
		int old_tail{ tail };
		tail = head + first;  
		if (tail >= capacity_) tail -= capacity_;
		push_back(buffer[old_tail]);
		size_ = first; //+
	}
	else if(first == 1 && last != size_){
		head += last;
		if (head >= capacity_) head -= capacity_;
		push_front(buffer[head]);
		size_ -= last; //+
	}
	else {
		if (head > tail) {
			linearize();
		}
		memcpy((value_type*)buffer + head + first - 1, (value_type*)buffer + head + last - 1, (tail - head - last + 2) * sizeof(value_type));
		tail -= last - first;
		size_ -= last - first;
	}
}

void CircularBuffer::clear() {
	buffer[0] = 0;
	size_ = 0;
	head = 0;
	tail = 0;
}

void CircularBuffer::push_front(const value_type& item){
	if(!empty()){
		if(--head < 0)
			head = capacity_ - 1;

		buffer[head] = item;
		if (size_ < capacity_) size_++;
		if (head < 0) head = capacity_ - 1;
		if (tail == head) tail--;
		if (tail < 0) tail = capacity_ - 1;
	}
	if(empty() && head == 0){
		buffer[tail] = item;
		if (size_ < capacity_) size_++;
	}
}

void CircularBuffer::push_back(const value_type& item) {
	if(!empty()){
		if(++tail >= capacity_)
			tail -= capacity_;
		buffer[tail] = item;
		if (size_ < capacity_) size_++;
		if (head == tail) head++;
		if (head >= capacity_) head = 0;
	}
	if(empty() && head == 0){
		buffer[tail] = item;
		if (size_ < capacity_) size_++;
	}
}

void CircularBuffer::pop_front() {
	if (!empty()) {
		buffer[head++] = 0;

		if(head >= capacity_)
			head = 0;
		size_--;
		if (tail == head)
			tail++;

		if(tail >= capacity_)
			tail = 0;
	}
}

void CircularBuffer::pop_back() {
	if (!empty()) {
		buffer[tail--] = 0;

		if(tail < 0)
			tail = capacity_ - 1;

		size_--;
		if (tail == head) {
			head--;
		}
		if(head < 0)
			head = capacity_ - 1;
	}
}

bool operator==(const CircularBuffer& a, const CircularBuffer& b) {
	if (a.size() != b.size()) 
		return false;
	if (a.capacity() != b.capacity()) 
		return false;

	for (int i = 0; i < a.size(); ++i) {
		if (a[i] != b[i]) 
			return false;
	}

	return true;
}

bool operator!=(const CircularBuffer& a, const CircularBuffer& b) {
	return !(a == b);
}
