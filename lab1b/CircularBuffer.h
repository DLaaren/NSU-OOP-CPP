#include <cstddef>
#include <stdexcept>

template <typename T>
class CircularBuffer {
private:
    int     capacity, 
            size;
    T      *buffer;
    int     start, 
            end; 
public:
    CircularBuffer();
    CircularBuffer(const CircularBuffer<T> & cb);
    explicit CircularBuffer(int capacity);
    CircularBuffer(int capacity, const T & elem);
    ~CircularBuffer();

    T & operator[](int i);
    const T & operator[](int i) const;
  
    T & at(int i);
    const T & at(int i) const;

    T & front();
    T & back();
    const T & front() const;
    const T & back() const;

    T * linearize();
    bool is_linearized() const;
    void rotate(int new_begin);
    int get_size() const;
    bool empty() const;
    bool full() const;
    int reserve() const;
    int get_capacity() const;

    void set_capacity(int new_capacity);
    void resize(int new_size, const T & item);
    CircularBuffer<T> & operator=(const CircularBuffer<T> & cb);
    void swap(CircularBuffer<T> & cb);

    void push_back(const T & item);
    void push_front(const T & item);
    void pop_back();
    void pop_front();

    void insert(int pos, const T & item);
    void erase(int first, int last);
    void clear();
};

/**
 * @brief Default constructor.
 * 
 * Initializes the circular buffer with a size of 0 and no allocated memory.
 */
template <typename T>
CircularBuffer<T>::CircularBuffer()
    : capacity(0), size(0), buffer(nullptr), start(-1), end(-1) {}

/**
 * @brief Copy constructor.
 * 
 * Creates a new circular buffer that is a copy of the given buffer.
 * 
 * @param cb The circular buffer to copy.
 */
template <typename T>
CircularBuffer<T>::CircularBuffer(const CircularBuffer<T> & cb)
    : capacity(cb.capacity), size(cb.size), start(cb.start), end(cb.end)
{
    buffer = new T[capacity];
    for (int i = 0; i < cb.size; i++) {
        buffer[(start + i) % capacity] = cb.buffer[(cb.start + i) % capacity];
    }
}

/**
 * @brief Constructor with specified capacity.
 * 
 * Initializes the circular buffer with the given capacity. The buffer size is initially 0.
 * 
 * @param capacity The capacity of the buffer.
 * @throws std::invalid_argument If capacity is non-positive.
 */
template <typename T>
CircularBuffer<T>::CircularBuffer(int capacity)
    : capacity(capacity), size(0), start(-1), end(-1)
{
    if (capacity <= 0) {
        throw std::invalid_argument("Capacity must be greater than 0");
    }

    buffer = new T[capacity];
}

/**
 * @brief Constructor with specified capacity and initial value.
 * 
 * Initializes the circular buffer with the given capacity, filling the buffer with the specified 
 * element.
 * 
 * @param capacity The capacity of the buffer.
 * @param elem The initial element to fill the buffer.
 */
template <typename T>
CircularBuffer<T>::CircularBuffer(int capacity, const T & elem)
    : CircularBuffer(capacity)
{
    for (int i = 0; i < capacity; i++) {
        buffer[i] = elem;
    }

    size = capacity;
    start = 0;
    end = size - 1;
}

/**
 * @brief Destructor.
 * 
 * Frees the allocated memory for the buffer.
 */
template <typename T>
CircularBuffer<T>::~CircularBuffer()
{
    delete[] buffer;
}

/**
 * @brief Access element at specified index (non-const).
 * 
 * Provides direct access to an element in the circular buffer using the operator[].
 * 
 * @param i The index of the element.
 * @return The element at the specified index.
 */
template <typename T>
T & CircularBuffer<T>::operator[](int i)
{
    return buffer[(start + i) % capacity];
}

/**
 * @brief Access element at specified index (const).
 * 
 * Provides direct access to an element in the circular buffer using the operator[].
 * 
 * @param i The index of the element.
 * @return The element at the specified index.
 */
template <typename T>
const T & CircularBuffer<T>::operator[](int i) const
{
    return buffer[(start + i) % capacity];
}

/**
 * @brief Access element at specified index with bounds checking (non-const).
 * 
 * Accesses an element at the given index with range checking.
 * 
 * @param i The index of the element.
 * @return The element at the specified index.
 * @throws std::out_of_range If index is out of range.
 */
template <typename T>
T & CircularBuffer<T>::at(int i)
{
    if (i < 0 || i >= size) {
        throw std::out_of_range("Index out of bounds");
    }
    return buffer[(start + i) % capacity];
}

/**
 * @brief Access element at specified index with bounds checking (const).
 * 
 * Accesses an element at the given index with range checking.
 * 
 * @param i The index of the element.
 * @return The element at the specified index.
 * @throws std::out_of_range If index is out of range.
 */
template <typename T>
const T & CircularBuffer<T>::at(int i) const
{
    if (i < 0 || i >= size) {
        throw std::out_of_range("Index out of bounds");
    }
    return buffer[(start + i) % capacity];
}

/**
 * @brief Get the element at the front of the buffer (non-const).
 * 
 * Retrieves the element at the start of the circular buffer.
 * 
 * @return The front element of the buffer.
 */
template <typename T>
T & CircularBuffer<T>::front()
{
    return buffer[start];
}

/**
 * @brief Get the element at the front of the buffer (const).
 * 
 * Retrieves the element at the start of the circular buffer.
 * 
 * @return The front element of the buffer.
 */
template <typename T>
const T & CircularBuffer<T>::front() const
{
    return buffer[start];
}

/**
 * @brief Get the element at the back of the buffer (non-const).
 * 
 * Retrieves the element at the end of the circular buffer.
 * 
 * @return The back element of the buffer.
 */
template <typename T>
T & CircularBuffer<T>::back()
{
    return buffer[end];
}

/**
 * @brief Get the element at the back of the buffer (const).
 * 
 * Retrieves the element at the end of the circular buffer.
 * 
 * @return The back element of the buffer.
 */
template <typename T>
const T & CircularBuffer<T>::back() const
{
    return buffer[end];
}

/**
 * @brief Convert the buffer to a linear array.
 * 
 * Converts the circular buffer to a non-circular array and reinitializes the buffer to a linear
 * layout.
 * 
 * @return A pointer to the new linearized buffer.
 */
template <typename T>
T * CircularBuffer<T>::linearize()
{
    if (size == 0) {
        return nullptr;
    }

    T* linearBuffer = new T[size];
    for (int i = 0; i < size; ++i) {
        linearBuffer[i] = buffer[(start + i) % capacity];
    }

    start = 0;
    end = size - 1;
    delete[] buffer;
    buffer = linearBuffer;

    return linearBuffer;
}

/**
 * @brief Check if the buffer is in linearized state.
 * 
 * Verifies if the buffer has been linearized (i.e., the start and end pointers match the 
 * linearized format).
 * 
 * @return True if the buffer is linearized, otherwise false.
 */
template <typename T>
bool CircularBuffer<T>::is_linearized() const
{
    return start == 0 && end == size - 1;
}

/**
 * @brief Rotate the buffer to a new start position.
 * 
 * Rotates the elements of the buffer to a new start index. The elements from the original 
 * start index are moved to the back.
 * 
 * @param new_begin The new start index for the buffer.
 * @throws std::out_of_range If the index is out of bounds.
 */
template <typename T>
void CircularBuffer<T>::rotate(int new_begin)
{
    if (new_begin < 0 || new_begin >= size) {
        throw std::out_of_range("Index out of bounds");
    }

    if (new_begin == start) {
        return;
    }

    int distance = new_begin - start;
    if (distance < 0) {
        distance += size;
    }

    T* new_buffer = new T[capacity];

    for (int i = 0; i < size; ++i) {
        new_buffer[i] = buffer[(start + i) % capacity];
    }

    start = 0;
    end = size - 1;

    delete[] buffer;
    buffer = new_buffer;
}

/**
 * @brief Get the current size of the buffer.
 * 
 * Returns the number of elements currently stored in the buffer.
 * 
 * @return The size of the buffer.
 */
template <typename T>
int CircularBuffer<T>::get_size() const
{
    return size;
}

/**
 * @brief Check if the buffer is empty.
 * 
 * Determines if the buffer is empty (i.e., contains no elements).
 * 
 * @return True if the buffer is empty, otherwise false.
 */
template <typename T>
bool CircularBuffer<T>::empty() const
{
    return size == 0;
}

/**
 * @brief Check if the buffer is full.
 * 
 * Determines if the buffer is full (i.e., has reached its capacity).
 * 
 * @return True if the buffer is full, otherwise false.
 */
template <typename T>
bool CircularBuffer<T>::full() const
{
    return size == capacity;
}

/**
 * @brief Get the remaining space in the buffer.
 * 
 * Returns the number of available slots in the buffer.
 * 
 * @return The available space in the buffer.
 */
template <typename T>
int CircularBuffer<T>::reserve() const
{
    return capacity - size;
}

/**
 * @brief Get the buffer's capacity.
 * 
 * Returns the total capacity of the buffer.
 * 
 * @return The capacity of the buffer.
 */
template <typename T>
int CircularBuffer<T>::get_capacity() const
{
    return capacity;
}

/**
 * @brief Set a new capacity for the buffer.
 * 
 * Changes the capacity of the buffer, reallocating memory for the new size.
 * 
 * @param new_capacity The new capacity to set.
 * @throws std::invalid_argument If the new capacity is non-positive.
 */
template <typename T>
void CircularBuffer<T>::set_capacity(int new_capacity)
{
    if (new_capacity <= 0) {
        throw std::invalid_argument("Capacity must be greater than 0");
    }

    capacity = new_capacity;
    this->buffer = realloc(this->buffer, new_capacity);
    if (this->buffer == nullptr) {
        throw std::out_of_range("Index out of bounds");
    }
}

/**
 * @brief Resize the buffer to a new size, filling the new elements with a specified value.
 * 
 * Changes the size of the buffer, adding or removing elements as necessary.
 * 
 * @param new_size The new size of the buffer.
 * @param item The value to fill new elements with (if increasing the size).
 * @throws std::out_of_range If the new size is invalid.
 */
template <typename T>
void CircularBuffer<T>::resize(int new_size, const T & item)
{
    if (new_size < 0 || new_size > capacity) {
        throw std::out_of_range("Invalid new size");
    }

    if (new_size > size) {
        for (int i = size; i < new_size; ++i) {
            push_back(item);
        }
    } else if (new_size < size) {
        for (int i = size - 1; i >= new_size; --i) {
            pop_back();
        }
    }
}

/**
 * @brief Assignment operator.
 * 
 * Performs a deep copy of the given buffer into the current buffer.
 * 
 * @param cb The circular buffer to copy from.
 * @return A reference to the current buffer.
 */
template <typename T>
CircularBuffer<T> & CircularBuffer<T>::operator=(const CircularBuffer<T> & cb)
{
    if (this != &cb) {
        delete[] buffer;

        capacity = cb.capacity;
        size = cb.size;
        start = cb.start;
        end = cb.end;

        buffer = new T[capacity];
        for (int i = 0; i < size; ++i) {
            buffer[(start + i) % capacity] = cb.buffer[(cb.start + i) % capacity];
        }
    }

    return *this;
}

/**
 * @brief Swap two buffers.
 * 
 * Exchanges the contents of the current buffer with the given buffer.
 * 
 * @param cb The circular buffer to swap with.
 */
template <typename T>
void CircularBuffer<T>::swap(CircularBuffer<T> & cb)
{
    T* temp = this->buffer;
    this->buffer = cb.buffer;
    cb.buffer = temp;
}

/**
 * @brief Add an element to the back of the buffer.
 * 
 * Inserts an element at the end of the buffer. If the buffer is full, it throws an exception.
 * 
 * @param item The element to add to the back of the buffer.
 * @throws std::overflow_error If the buffer is full.
 */
template <typename T>
void CircularBuffer<T>::push_back(const T & item)
{
    if (full()) {
        throw std::overflow_error("Buffer is full");
    }
    end = (end + 1) % capacity;
    buffer[end] = item;
    if (size == 0)
    {
        start = 0;
    }
    size++;
}

/**
 * @brief Add an element to the front of the buffer.
 * 
 * Inserts an element at the front of the buffer. If the buffer is full, it throws an exception.
 * 
 * @param item The element to add to the front of the buffer.
 * @throws std::overflow_error If the buffer is full.
 */
template <typename T>
void CircularBuffer<T>::push_front(const T & item)
{
    if (full()) {
        throw std::overflow_error("Buffer is full");
    }
    start = (start - 1 + capacity) % capacity;
    buffer[start] = item;
    if (size == 0)
    {
        start = 0;
    }
    size++;
}

/**
 * @brief Remove an element from the back of the buffer.
 * 
 * Removes the last element in the buffer. If the buffer is empty, it throws an exception.
 * 
 * @throws std::underflow_error If the buffer is empty.
 */
template <typename T>
void CircularBuffer<T>::pop_back()
{
    if (empty()) {
        throw std::underflow_error("Buffer is empty");
    }
    end = (end - 1 + capacity) % capacity;
    size--;
    if (size == 0)
    {
        start, end = -1, -1;
    }
}

/**
 * @brief Remove an element from the front of the buffer.
 * 
 * Removes the first element in the buffer. If the buffer is empty, it throws an exception.
 * 
 * @throws std::underflow_error If the buffer is empty.
 */
template <typename T>
void CircularBuffer<T>::pop_front()
{
    if (empty()) {
        throw std::underflow_error("Buffer is empty");
    }
    start = (start + 1) % capacity;
    size--;
    if (size == 0)
    {
        start, end = -1, -1;
    }
}

/**
 * @brief Insert an element at a specific position in the buffer.
 * 
 * Inserts an element at the given position in the buffer. If the buffer is full, or if the 
 * position is out of range, it throws an exception.
 * 
 * @param pos The position at which to insert the element.
 * @param item The element to insert.
 * @throws std::out_of_range If the position is out of bounds.
 * @throws std::overflow_error If the buffer is full.
 */
template <typename T>
void CircularBuffer<T>::insert(int pos, const T & item)
{
    if (pos < 0 || pos > size) {
        throw std::out_of_range("Index out of bounds");
    }

    if (full()) {
        throw std::overflow_error("Buffer is full");
    }

    for (int i = size - 1; i >= pos; --i) {
        buffer[(start + i + 1) % capacity] = buffer[(start + i) % capacity];
    }

    buffer[(start + pos) % capacity] = item;
    ++size;
}


/**
 * @brief Erase a range of elements from the buffer.
 * 
 * Removes elements from the buffer between the specified indices. If the indices are out of 
 * range, it throws an exception.
 * 
 * @param first The start index of the range to remove.
 * @param last The end index of the range to remove.
 * @throws std::out_of_range If the range is invalid.
 */
template <typename T>
void CircularBuffer<T>::erase(int first, int last)
{
    if (first < 0 || last >= size || first > last) {
        throw std::out_of_range("Invalid range");
    }

    // Shift elements to erase the specified range
    for (int i = last + 1; i < size; ++i) {
        buffer[(start + i - (last - first + 1)) % capacity] = buffer[(start + i) % capacity];
    }

    size -= (last - first + 1);
    end = (end - (last - first + 1) + capacity) % capacity;
}

/**
 * @brief Clear all elements from the buffer.
 * 
 * Removes all elements from the buffer, resetting its size, start, and end pointers to their 
 * initial values.
 */
template <typename T>
void CircularBuffer<T>::clear()
{
    size = 0;
    start = 0;
    end = 0;
}

/**
 * @brief Equality comparison operator.
 * 
 * Compares two circular buffers for equality. Two buffers are considered equal if they have 
 * the same size and their elements are in the same order.
 * 
 * @param a The first circular buffer to compare.
 * @param b The second circular buffer to compare.
 * @return True if the buffers are equal, otherwise false.
 */
template <typename T>
bool operator==(const CircularBuffer<T>& a, const CircularBuffer<T>& b)
{
    if (a.get_size() != b.get_size()) {
        return false;
    }

    for (int i = 0; i < a.get_size(); ++i) {
        if (a[(a.front() + i) % a.get_capacity()] != b[(b.front() + i) % b.get_capacity()]) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Inequality comparison operator.
 * 
 * Compares two circular buffers for inequality. Two buffers are considered unequal if they 
 * are not equal according to the equality comparison operator.
 * 
 * @param a The first circular buffer to compare.
 * @param b The second circular buffer to compare.
 * @return True if the buffers are not equal, otherwise false.
 */
template <typename T>
bool operator!=(const CircularBuffer<T>& a, const CircularBuffer<T>& b)
{
    return !(a == b);
}
