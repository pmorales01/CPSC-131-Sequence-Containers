#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iomanip>
#include <iterator>
#include <stdexcept>
#include <string>

#include "book.hpp"
#include "book_list.hpp"

bool BookList::containers_are_consistent() const {
  // If the sizes of the containers are not all equal, the containers are not
  // consistent.
  if (books_array_size_ != books_vector_.size()
      || books_array_size_ != books_dl_list_.size()
      || books_array_size_ !=  books_sl_list_size()) {
    return false;
  }

  // Element content and order must be equal to each other
  auto current_array_position = books_array_.cbegin();
  auto current_vector_position = books_vector_.cbegin();
  auto current_dl_list_position = books_dl_list_.cbegin();
  auto current_sl_list_position = books_sl_list_.cbegin();

  while (current_vector_position != books_vector_.cend()) {
    if (*current_array_position != *current_vector_position
        || *current_array_position != *current_dl_list_position
        || *current_array_position != *current_sl_list_position) {
      return false;
    }

    // Advance the iterators to the next element in unison
    ++current_array_position;
    ++current_vector_position;
    ++current_dl_list_position;
    ++current_sl_list_position;
  }

  return true;
}

std::size_t BookList::books_sl_list_size() const {
    // Some implementations of a singly-linked list maintain the size
    // (number of elements in the list). The std::forward_list class does not.
    // The size of the singly-linked list must be calculated on demand by
    // walking the list from beginning to end counting the number of elements
    // visited. The STL's std::distance() function does that, or you can write
    // your own loop.

    // Since we want to start calculating the distance from the beginning of the
    // std::forward_list to to the end, the first argument to std::distance() is going
    // to be be books_sl_list_.begin() and the second argument is  books_sl_list_.end().

    // books_sl_list_.begin() returns an iterator pointing to the beginning of the 
    // singly-linked list, and books_sl_list_.end() returns an iterator pointing to the
    // end of the singly-linked list (which is the element past the last element in the SLL).

    // Using the std::distance() calculates the number of elements in the singly-linked 
    // list, so therefore the size. 

    // returns the singly-linked list size
    return std::distance(books_sl_list_.begin(), books_sl_list_.end());
}

//
// Constructors, Assignments, and Destructor
//

BookList::BookList() = default;

BookList::BookList(const BookList& other) = default;

BookList::BookList(BookList&& other) = default;

BookList& BookList::operator=(const BookList& rhs) = default;

BookList& BookList::operator=(BookList&& rhs) = default;

BookList::~BookList() = default;

BookList::BookList(const std::initializer_list<Book>& init_list) {
  for (const Book& book : init_list) {
    insert(book, Position::BOTTOM);
  }

  // Verify the internal book list state is still consistent amongst the four
  // containers.
  if (!containers_are_consistent()) {
    throw BookList::InvalidInternalStateException(
        "Container consistency error in initializer_list constructor");
  }
}

BookList& BookList::operator+=(const std::initializer_list<Book>& rhs) {
    // Concatenate the right hand side book list of books to this list by
    // repeatedly inserting at the bottom of this book list.
    //
    // The input type is a container of books accessible with iterators like
    // all the other containers. The constructor above gives an example. Use
    // BookList::insert() to insert at the bottom.

    // used a range based for loop to create the variable book that will be used to 
    // iterate over rhs list elements

    for (const Book& book : rhs) {
      // we insert each element, book, at the bottom of BookListand this repeats
      insert(book, Position::BOTTOM);
    }
    


  // Verify the internal book list state is still consistent amongst the four containers.
  if (!containers_are_consistent()) {
    throw BookList::InvalidInternalStateException(
        "Container consistency error in operator+= for initializer list");
  }
  return *this;
}

BookList& BookList::operator+=(const BookList& rhs) {
    // Concatenate the right hand side book list of books to this list by
    // repeatedly inserting at the bottom of this book list.
    //
    // All the rhs containers (array, vector, list, and forward_list) contain
    // the same information, so pick just one to traverse. Walk the container 
    // you picked, inserting its books to the bottom of this book list. Use
    // BookList::insert() to insert at the bottom.
    
    // Uses a range based for loop and the rhs container, vector,
    // to concatenate the rhs book list.

    // Each time the loop iterates, a book from rhs.books_vector_ is assigned to book.
    for (const Book& book : rhs.books_vector_) {
      // While the for loop executes, uses BookList::insert() to insert book to the 
      // bottom of the book list
      insert(book, Position::BOTTOM);
    }
    


  // Verify the internal book list state is still consistent amongst the four
  // containers.
  if (!containers_are_consistent()) {
    throw BookList::InvalidInternalStateException(
        "Container consistency error in operator+= for BookList");
  }
  return *this;
}

//
// Queries
//

std::size_t BookList::size() const {
  // Verify the internal book list state is still consistent amongst the four
  // containers.
  if (!containers_are_consistent()) {
    throw BookList::InvalidInternalStateException(
      "Container consistency error in size");
  }

    // All the containers are the same size, so pick one and return the size of
    // that container.
    //
    // Since the forward_list has to calculate its size on demand, stay away
    // from using that one.

    // Since std::vector has a size() function, its convenient to use it to return
    // the size of the vector container. 

    // Returns the size of the vector container.  
    return  books_vector_.size();
}

std::size_t BookList::find(const Book& book) const {
  // Verify the internal book list state is still consistent amongst the four
  // containers.
  if (!containers_are_consistent()) {
    throw BookList::InvalidInternalStateException(
        "Container consistency error in find");
  }

    // Locate the book in this book list and return the zero-based position of
    // that book. If the book does not exist, return the size of this book list
    // as an indicator the book does not exist.
    //
    // The book will be in the same position in all the containers
    // (array, vector, list, and forward_list) so pick just one of those to
    // search. The STL provides the find() function that is a perfect fit here,
    // but you may also write your own loop.

    std::vector<Book>::const_iterator iter = std::find(books_vector_.begin(), books_vector_.end(), book);
    if (iter != books_vector_.end()) {
      // iter is now pointing at the right object
      // vector.begin() returns an iterator pointing to the first vector element
      // To get the number of positions traveled
      // I would find the difference between the two iterators
      // in the case of zero-based positions
      // the distance from the beginning is the correct position
      return iter - books_vector_.begin(); 
    } else {
      // iter == books_vector_.end(), this means that book was not found.
      // Since book was not found, the size of this book list is returned. 
      return size();
    }
}

//
// Mutators
//

BookList& BookList::insert(const Book& book, Position position) {
  // Convert the TOP and BOTTOM enumerations to an offset and delegate the work.
  switch (position) {
    case Position::TOP: {
      insert(book, 0);
      break;
    }
    case Position::BOTTOM: {
      insert(book, size());
      break;
    }
  }
  return *this;
}

// Insert the new book at offset_from_top, which places it before the current
// book at that position.
BookList& BookList::insert(const Book& book, std::size_t offset_from_top) {
  // Validate offset parameter before attempting the insertion. As std::size_t
  // is an unsigned type, there is no need to check for negative offsets. And an
  // offset equal to the size of the list says to insert at the end (bottom) of
  // the list. Anything strictly greater than the current size is an error.
  if (offset_from_top > size()) {
    throw InvalidOffsetException(
        "Insertion position beyond end of current list size in insert");
  }

  //
  // Prevent duplicate entries
  //

    // Silently discard duplicate items from getting added to the book list.
    //
    // If the to-be-inserted book is already in the list, simply return.
  
    // We can use the already implemented BookList::find() function to determine if book
    // is already in the book list. 

    // BookList::find() returns the size of book list if the book was not found or the zero-based position of the book if the book is found. 

    // This if-statement is true if the zero-based position of book is found. 
    if (find(book) != size()) { 
      // A zero-based position means that the book alreadys exists and is a duplicate. 

      // Return the current instance of BookList since a duplicate was found.
      // This simulates discarding a duplicate book. 
      return *this;
    }

  // Inserting into the book list means you insert the book into each of the
  // containers (array, vector, forward_list, and list).
  //
  // Because the data structure concept is different for each container, the
  // way a book gets inserted is a little different for each. You are to insert
  // the book into each container such that the ordering of all the containers
  // is the same. A check is made at the end of this function to verify the
  // contents of all four containers are indeed the same.

  //
  // Insert into array
  //

  {
      // Unlike the other containers, std::array has no insert() function, so
      // you have to write it yourself. Insert into the array by shifting all
      // the items at and after the insertion point (offset_from_top) to the
      // right, opening a gap in the array that can be populated with the given
      // book.
      //
      // Remember that arrays have fixed capacity and cannot grow, so make sure 
      // there is room in the array for another book before you start by
      // verifying books_array_size_ is less than books_array_.size(). If
      // not, throw a CapacityExceededException exception.
      //
      // Also remember that you must keep track of the number of valid books in
      // your array, so don't forget to adjust books_array_size_.
      //

      // To check that there is enough room in the array for another book, we checked
      // if books_array_size_ is greater than or equal to books_array_.size() because 
      // that means books_array_ is too small to hold another book if true. 
      if (books_array_size_ >= books_array_.size()) {
        throw CapacityExceededException("Capacity Exceeded. books_array_size_ is less thanbooks_array_.size()");
      }

      // To add the book, first create a gap at the index where offset_from_top will be added.
      // All the elements at the indexes that are less than offset_from_top will not be changed.
      // All the elements at indexes greater than or equal to offset_from_top will be changed.

      // int i in the for loop is initialized to books_array_size_ to represent the number of
      // books that are in books_array_. 
      // The loop iterates until int i (the index) is greater than offset_from_top. 
      // Each time the loop iterates, i is decremented.
      for (int i = books_array_size_; i > offset_from_top; --i) {
        //  Starting from i = book_array_size_ ensures that all the elements to the right
        //  of the gap are shifted one index to the right and copied.

        // Each time the loop iterates, we take the element from the index that comes before 
        // the current index and assign it to the current index. 

        books_array_[i] = books_array_[i - 1];

        // The loop stops incrementing when i is equal to offset_from_top
      }

      // Inserts the new book to the books_array_ since the loop did not insert it. 
      books_array_[offset_from_top] = book;

      // Increment size to represent a new book added to the array. 
      books_array_size_++;
  }

  //
  // Insert into vector
  //

  {
      // The vector STL container std::vector has an insert function, which can
      // be directly used here. But that function takes an iterator that points
      // to the book to insert before. You need to convert the zero-based
      // offset from the top to an iterator by advancing books_vector_.begin()
      // offset_from_top many times. The STL has a function called std::next() 
      // that does that, or you can use simple pointer arithmetic to calculate
      // it.
      //
      // Behind the scenes, std::vector::insert() shifts to the right
      // everything at and after the insertion point, just like you
      // did for the array above.

      // Creates an iterator pointing to the beginning of books_vector_.
      std::vector<Book>::iterator v_iter = books_vector_.begin();

      // Mutates the iterator by advancing v_iter offset_from_top many times.
      v_iter = std::next(v_iter, offset_from_top);

      // book is inserted to the vector.
      books_vector_.insert(v_iter, book);

  }

  //
  // Insert into singly-linked list
  //

  {
      // The singly-linked list STL container std::forward_list has an insert
      // function, which can be directly used here. But that function inserts
      // AFTER the book pointed to, not before like the other containers. A
      // singly-linked list cannot look backwards, only forward. You need to
      // convert the zero-based offset from the top to an iterator by advancing
      // books_sl_list_.before_begin() offset_from_top many times. The STL has
      // a function called std::next() that does that, or you can write your own
      // loop.
      // std::forward_list<Book> books_sl_list_;

      // Creates an iterator of type std::forward_list to point before the beginning of books_sl_list_.
      std::forward_list<Book>::iterator sl_iter = books_sl_list_.before_begin();

      // Mutates sl_iter by advancing sl_iter offset_from_top many times. 
      sl_iter = std::next(sl_iter, offset_from_top);

      // Uses std::forward_list::insert_after() to insert book after sl_iter. 
      books_sl_list_.insert_after(sl_iter, book);
  }

  //
  // Insert into doubly-linked list
  //

  {
      // The doubly-linked list STL container std::list has an insert function,
      // which can be directly used here. But that function takes an iterator
      // that points to the book to insert before. You need to convert the
      // zero-based offset from the top to an iterator by advancing
      // books_dl_list_.begin() offset_from_top times. The STL has a function
      // called std::next() that does that, or you can write your own loop.
      
      // Creates an iterator of type std::list to point to the beginning of books_dl_list_
      std::list<Book>::iterator dl_iter = books_dl_list_.begin();

      // Mutates dl_iter by advancing dl_iter offset_from_top many times. 
      dl_iter = std::next(dl_iter, offset_from_top);

      // Uses std::list::insert() to insert book before dl_iter
      books_dl_list_.insert(dl_iter, book);
  }

  // Verify the internal book list state is still consistent amongst the four
  // containers.
  if (!containers_are_consistent()) {
    throw BookList::InvalidInternalStateException(
        "Container consistency error in insert");
  }
  return *this;
}

BookList& BookList::remove(const Book& book) {
  remove(find(book));
  return *this;
}

BookList& BookList::remove(std::size_t offset_from_top) {
  // Removing from the book list means you remove the book from each of the
  // containers (array, vector, list, and forward_list).
  //
  // Because the data structure concept is different for each container, the
  // way a book gets removed is a little different for each. You are to remove
  // the book from each container such that the ordering of all the containers
  // is the same. A check is made at the end of this function to verify the
  // contents of all four containers are indeed the same.

  // If offsetFromTop isn't a valid offset, no change occurs.
  if (offset_from_top >= size()) {
    return *this;
  }

  //
  // Remove from array
  //

  {
      // Close the hole created by shifting to the left everything at and after
      // the remove point.
      //
      // std::move() will be helpful, or write your own loop.  Also remember
      // that you must keep track of the number of valid books in your array,
      // so don't forget to adjust books_array_size_.
      //

      // there are 3 arguments for std::move(), each is an iterator 
      
      // The first two arguments are responsible for creating a range of elements 
      // that we want to move 

      // The third argument is the location or position where we want to begin moving 
      // the elements from the range into. 

      // Creates an iterator pointing to offset_from_top + 1 positions away from the beginning
      // of the array. This iterator represents the beginning of the range of elements we want 
      // to move. 

      // The range starts at this position because we don't want to move anything to the left of
      // offset_from_top, and we don't want to include the element at offset_from_top because
      // we want to remove it from the array. So we start the range of the elements at one 
      // position after offset_from_top because that's the first element we want to include. 
      auto range_start_iter = books_array_.begin() + offset_from_top + 1;

      // Creates an iterator pointing to the end of books_array_. This represents the end of 
      // the range of elements we want to move. 

      auto range_end_iter = books_array_.end();

      // Creates an iter that points to the location where we starting moving the elements to.  
      // Since we want to keep the elements that are left of offset_from_top and we want to remove
      // the element at offset_from_top, we start iterating at offset_from_top positions from
      // the beginning of the books_array_.

      auto first_destination = books_array_.begin() + offset_from_top;

      // Uses std::move() to move the range of elements into first_destination. 
      std::move(range_start_iter, range_end_iter, first_destination);

      // Decrement books_array_size_ to represent removing a book from the array
      books_array_size_--;
    
  }

  //
  // Remove from vector
  //

  {
      // The vector STL container std::vector has an erase function, which can
      // be directly used here. But that function takes an iterator that points
      // to the book to be removed. You need to convert the zero-based offset
      // from the top to an iterator by advancing books_vector_.begin()
      // offset_from_top times. The STL has a function called std::next() that
      // does that, or you can use simple pointer arithmetic to calculate it.
      //
      // Behind the scenes, std::vector::erase() shifts to the left everything 
      // after the insertion point, just like you did for the array above.
    
      // Creates an iterator of type std::vector<Book> that is initialized with a position that has
      // been advanced offset_from_top many times from the beginning of books_vector_ by using std::next().
      std::vector<Book>::iterator vector_position = std::next(books_vector_.begin(), offset_from_top);

      // Uses std::vector::erase() to remove the element that vector_position is pointing to. 
      books_vector_.erase(vector_position);
  }

  //
  // Remove from singly-linked list
  //

  {
      // The singly-linked list STL container std::forward_list has an erase
      // function, which can be directly used here. But that function erases
      // AFTER the book pointed to, not the one pointed to like the other
      // containers. A singly-linked list cannot look backwards, only forward.
      // You need to convert the zero-based offset from the top to an iterator
      // by advancing books_sl_list_.before_begin() offset_from_top many times.
      // The STL has a function called std::next() that does that, or you can
      // write your own loop.

      // Creates an iterator of type std::forward_list<Book> that is initialized with a position 
      // that has been advanced offset_from_top many times from before the beginning of books_sl_list_. 
      std::forward_list<Book>::iterator sl_position_iter = std::next(books_sl_list_.before_begin(), offset_from_top);

      // Uses the std::forward_list::erase_after() function to remove the element after sl_position_iter. 
      books_sl_list_.erase_after(sl_position_iter);
  }

  //
  // Remove from doubly-linked list
  //

  {
      // The doubly-linked list STL container std::list has an erase function,
      // which can be directly used here. But that function takes an iterator
      // that points to the book to remove. You need to convert the zero-based
      // offset from the top to an iterator by advancing books_dl_list_.begin()
      // offset_from_top many times. The STL has a function called std::next()
      // that does that, or you can write your own loop.
    
      // Creates an iterator of type std::list<Book> that is initialized with a position 
      // that has been advanced offset_from_top many times from the beginning of books_dl_list_ by
      // using std::next().
      std::list<Book>::iterator dl_position_iter = std::next(books_dl_list_.begin(), offset_from_top);

      // Uses std::list::erase() to remove the element that dl_position_iter is pointing to. 
      books_dl_list_.erase(dl_position_iter);

  }

  // Verify the internal book list state is still consistent amongst the four
  // containers.
  if (!containers_are_consistent()) {
    throw BookList::InvalidInternalStateException(
        "Container consistency error in remove");
  }
  return *this;
}

BookList& BookList::move_to_top(const Book& book) {

    // If the book exists, then remove and reinsert it. Else do nothing.
    //
    // Use BookList::find() to determine if the book exists in this book list.

    // When using BookList::find(), we know if a book does not exist when size() is returned.
    // When the if statement does not return size(), that means that the book exists and we should move it to the top.
    if (find(book) != size()) { // if book exists
      // Remove the book by using BookList::remove().
      remove(book);
      // Insert the book to the top of BookList by using BookList::insert().
      insert(book, 0);
    }


  // Verify the internal book list state is still consistent amongst the four
  // containers.
  if (!containers_are_consistent()) {
    throw BookList::InvalidInternalStateException(
        "Container consistency error in move_to_top");
  }
  return *this;
}

void BookList::swap(BookList& rhs) noexcept {
  if (this == &rhs) {
    return;
  }

  books_array_.swap(rhs.books_array_);
  books_vector_.swap(rhs.books_vector_);
  books_dl_list_.swap(rhs.books_dl_list_);
  books_sl_list_.swap(rhs.books_sl_list_);

  std::swap(books_array_size_, rhs.books_array_size_);
}

//
// Insertion and Extraction Operators
//

std::ostream& operator<<(std::ostream& stream, const BookList& book_list) {
  if (!book_list.containers_are_consistent()) {
    throw BookList::InvalidInternalStateException(
        "Container consistency error in operator<<");
  }

  int count = 0;
  stream << book_list.size();
  for (auto&& book : book_list.books_sl_list_) {
    stream << '\n' << std::setw(5) << count++ << ":  " << book;
  }
  stream << '\n';
  return stream;
}

std::istream& operator>>(std::istream& stream, BookList& book_list) {
  if (!book_list.containers_are_consistent()) {
    throw BookList::InvalidInternalStateException(
        "Container consistency error in operator>>");
  }
  std::string label_holder;
  size_t count;

    // Read in data from a stream and use it to fill in the data of a BookList
    // object. Create a function that will read the output created by the
    // ostream operator above into an object properly.
    
    // reads the istream data to the count variable 
    stream >> count;
    // discards the next character that is read 
    stream.ignore(1);

    // The while loop makes sure that only books_sl_list_size number of books are extracted 
    // to avoid extracting any extra books. 
    while (count != book_list.books_sl_list_size()) {
       // Creates a temporary book object so that the istream data can be extracted to it 
      Book temp_book;
      
      // discards/ignores the next 4 characters
      stream.ignore(4);

      // reads the istream data to temp_book 
      stream >> temp_book;

      // insert temp_book into the booklist
      book_list.insert(temp_book, BookList::Position::BOTTOM);
    }
    
  

  return stream;
}

//
// Relational Operators
//

int BookList::compare(const BookList& other) const {
  if (!containers_are_consistent() || !other.containers_are_consistent()) {
    throw BookList::InvalidInternalStateException(
        "Container consistency error in compare");
  }


    // Compare this Booklist object with the other Booklist object. Return:
    // * a negative number if this Booklist object is less than the other 
    //   Booklist object,
    // * zero if this Booklist object is equal to the other Booklist object, and
    // * a positive number if this Booklist object is greater than the other
    //   Booklist object.
    //
    // Compare the size of the two Booklist objects first. If the sizes are
    // different, you have your answer: if size() < other.size() then return -1,
    // and so on.
    //
    // If the sizes are equal, then walk the lists from beginning to end
    // comparing the contents of the two books, one from this BookList and one
    // from the other BookList. The content of all the containers in a
    // BookList is the same - so pick one to walk. If the books are different,
    // you have your answer. If you get all the way to the end of the list and
    // found no books that are different, you have your answer: the lists are 
    // equal.

    // checks if the size of the booklist object is unequal to the other obooklist object. 
    // If false, the booklist sizes are equal. 
  if (size() != other.size()) {
    // if this book list is smaller than the other booklist object we return -1
    if (size() < other.size()) {
      return -1;
    } 
    // if this book list is bigger than the other booklist object we return 1
    if (size() > other.size()){
      return 1;
    }
  }

  // Creates a const iterator of type std::vector<Book> to point to the beginning of other's books_vector_. 
  std::vector<Book>::const_iterator other_iter = other.books_vector_.begin();

  // Uses a for loop to iterate through the books_vector_ of this BookList object and other's. 

  // vector_iter is created by using auto and is initialized to point at the beginning of books_vector_. 

  // Each time the loop iterates, the iterators of this BookList and other mutate to point to the next
  // position in the books_vector_.
  for (auto vector_iter = books_vector_.begin(); 
            vector_iter != books_vector_.end();
            vector_iter++) {
    // The two conditionals compare the contents at corresponding positions
    if (*vector_iter < *other_iter) { // return -1 if this BookList's book is less than other's book
      return -1;
    } 
    if (*vector_iter > *other_iter) { // return 1 if this Booklist's book is more than the other's book
      return 1;
    }
    // if neither condition is met we look at the next book 
    other_iter++;
  }
  //return zero if the book lists are equal
  
  return 0;
  
}

bool operator==(const BookList& lhs, const BookList& rhs) {
  return lhs.compare(rhs) == 0;
}

bool operator!=(const BookList& lhs, const BookList& rhs) {
  return lhs.compare(rhs) != 0;
}

bool operator<(const BookList& lhs, const BookList& rhs) {
  return lhs.compare(rhs) < 0;
}

bool operator<=(const BookList& lhs, const BookList& rhs) {
  return lhs.compare(rhs) <= 0;
}

bool operator>(const BookList& lhs, const BookList& rhs) {
  return lhs.compare(rhs) > 0;
}

bool operator>=(const BookList& lhs, const BookList& rhs) {
  return lhs.compare(rhs) >= 0;
}