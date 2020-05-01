    // Move constructor for rvalue, and copy constructor for lvalue
   // In the Bst class, we have the following  public functions :

        //begin                                                                     :ajoutee
        //end                                                                       :ajoutee
        //insert                                                                    :ajoutee
        //emplace                                                                   :ajoutee
        //find                                                                      :ajoutee
        //clear                                                                     :ajoutee
        //balance                                                                   :ajoutee
        //friend std::ostream& operator<<(std::ostream& os, const bst& x);          :ajoutee
        //value_type& operator[](const key_type& x);
        //value_type& operator[](key_type&& x);

    // And the following private functions :

        //_begin                                                                     :ajoutee
        //_insert                                                                    :ajoutee
        //_find                                                                      :ajoutee
        //_find_or_insert                                                            :ajoutee

        //balance



#include <functional>
#include <memory>  /* std::unique_ptr */
#include <utility> /* std::move, std::pair */
//#include <cstddef>  /* std::ptrdiff_t */
//#include <iterator> /* std::forward_iterator_tag */
#include <iostream>
#include <vector>


using namespace std;
	
//template <typename T>


template < typename T >
struct _node{
      
        T value;
        _node * left = nullptr;
        _node * right = nullptr;
        _node * parent  = nullptr;
    
        explicit _node(const T& x, _node* y) noexcept : value{x}, parent{y} {}  
        explicit _node(T&& x, _node* y) noexcept: value{std::move(x)}, parent{y} {}
        explicit _node(const T& x ) noexcept : value{x} {}
        explicit _node(T&& x ) noexcept: value{std::move(x)} {}
    };//End of the struct BST



template <typename T>
T* _left_most(T* x) 
{
    if (!x)
        return x;
    while (x->left)
        x = x->left;
    return x;
}

template <typename T>
T* _left_most(const std::unique_ptr<T>& x) 
{
    return _left_most(x.get());
}

template <typename T>
T* _next(T* current) 
{
    if (!current)
        return current;
    auto parent = current->parent;
    while (parent) 
    {
        if (parent->left == current)
            break;
        current = parent;
        parent  = parent->parent;
    }
    return parent;
}

template <typename node_t, typename B>
class _iterator{
public:
    using pair = B;
    using node = node_t;
    using reference         = pair&;
    using pointer           = pair*;
 //   using difference_type   = std::ptrdiff_t;
 //   using iterator_category = std::forward_iterator_tag;
  

    _iterator(node_t* x) : current{x} {}    
    
    
    friend bool operator==(const _iterator& x, const _iterator& y) 
    {        return x.current == y.current;    }
  
    friend bool operator!=(const _iterator& x, const _iterator& y) 
    {        return !(x == y);      }


    reference operator*() const noexcept { return current->value; }
    pointer operator->() const noexcept { return &**this; }
    _iterator& operator++() noexcept
    {   // it return an iterator = a pointer to node + all the iterator operations
  
    if (current->right)
        current = _left_most(current->right);
    else
        current = _next(current);
    return *this;  
  
    }

private:
    node_t * current;

};//End of the class _iterartor


template <typename key_t, typename value_t, typename cmp_t = std::less<key_t>>
class bst{
	
public:

    using pair_type         = std::pair<const key_t, value_t>;
    using node_type         = _node<pair_type>;
    using iterator          = _iterator<node_type, pair_type>;
    using const_iterator    = _iterator<node_type, const pair_type>;

    bst() noexcept  = default;
    bst(bst&&) noexcept = default;  // move semantique doesn't work without noexcept
    bst(const bst& x) : op{x.op} 
    {
        if (x.root)
            root = new node_type(x.root, nullptr);
    }

    ~bst() noexcept { clear(); }

    ///////////////////public functions of bst class///////////////////////////////////////////

    std::pair<iterator, bool> insert(const pair_type& x) { return _insert(x,root,nullptr); }
    std::pair<iterator, bool> insert(pair_type&& x) {return _insert(std::move(x),root,nullptr); }

    template <typename... Types>   
    std::pair<iterator, bool> emplace(Types&&... args) { return insert({std::forward<Types>(args)...});}


    iterator find(const key_t& x) noexcept { return _find(x); }
    const_iterator find(const key_t& x) const noexcept { return _find(x); }
   
    iterator begin() noexcept { return _begin(); }
    const_iterator begin() const noexcept { return _begin(); };
    const_iterator cbegin() const noexcept { return begin(); }

    iterator end() noexcept { return nullptr; }
    const_iterator end() const noexcept { return nullptr; }
    const_iterator cend() const noexcept { return end(); }


    void balance(); 

    void clear() { _clear( root ); }

    //void clear() noexcept { root.reset(); }

    value_t& operator[](const key_t& x) { return _find_or_insert(x); }
    value_t& operator[](key_t&& x) { return _find_or_insert(std::move(x)); }

private:
    node_type * root;
    cmp_t op;

    std::pair< iterator , bool > _insert(pair_type&& x, node_type*& p, node_type* par); 
    node_type* _find(const key_t& x) const noexcept;
    node_type* _begin() const noexcept { return _left_most(root); }

    void _insert_recursive( node_type* x,
                            const std::vector<pair_type>& pairs,
                            const std::size_t l_begin,
                            const std::size_t l_end,
                            const std::size_t r_begin,
                            const std::size_t r_end); 

    template <typename T>
    value_t& _find_or_insert(T&& x) 
    {
        auto it = find(x);
        if (it != end())
            return it->second;
        return emplace(std::forward<T>(x), value_t{}).first->second;
    }

    void _clear( node_type*& t );

    friend std::ostream& operator<<(std::ostream& os, const bst& tree) 
    {
        for (const auto& x : tree)
            os << "(" << x.first << "," << x.second << ")" << " ";
        return os;
    }

};//End of the class BST

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




template <typename key_t, typename value_t, typename cmp_t>
std::pair< typename bst<key_t, value_t, cmp_t>::iterator, bool > 
bst<key_t, value_t, cmp_t>::_insert(pair_type&& x, node_type*& p, node_type* par)  
{  
    if( !p )
    {
        p = new node_type( x , par );
        return { p , true };
    }

    else
    {
        const auto& k = p->value.first;
           
        if      ( op( x.first , k ) )   return _insert(std::move(x), ((p)->left), p);
        else if ( op( k , x.first ) )   return _insert(std::move(x), ((p)->left), p);
        else                            return {nullptr, false};
    }
}



template <typename key_t, typename value_t, typename cmp_t>
typename bst<key_t, value_t, cmp_t>::node_type*
bst<key_t, value_t, cmp_t>::_find(const key_t& x) const noexcept {
    auto tmp = root;
    while (tmp) {
        const auto& k = tmp->value.first;
        if (op(x, k))
            tmp = tmp->left;
        else if (op(k, x))
            tmp = tmp->right;
        else
            break;
        }
    return tmp;
}
//////////////////////////balance()/////////////////////////////////////////////

template <typename key_t, typename value_t, typename cmp_t>
void bst<key_t, value_t, cmp_t>::balance()
{ 
    if (!root)
        return;


    std::vector<pair_type> pairs; // store the bst in the vector pairs

    auto first = cbegin();
    auto last  = cend();
    while (first != last) {
      pairs.emplace_back(*first);
      ++first;
    }
    clear();  // free the originel bst

    const std::size_t half = pairs.size() / 2;
    root = new node_type(pairs[half], nullptr);
    

    _insert_recursive(root, pairs, 0, half, half + 1, pairs.size());

}  
template <typename key_t, typename value_t, typename cmp_t>
void bst<key_t, value_t, cmp_t>::_insert_recursive( node_type* x,
                                                    const std::vector<typename bst<key_t, value_t, cmp_t>::pair_type>& pairs,
                                                    const std::size_t l_begin,
                                                    const std::size_t l_end,
                                                    const std::size_t r_begin,
                                                    const std::size_t r_end) 
{
    if (l_begin != l_end) 
    {
        const std::size_t half = (l_begin + l_end) / 2;
        x->left                = new node_type(pairs[half], x);
        _insert_recursive(x->left, pairs, l_begin, half, half + 1, l_end);
    }

  //  if (r_begin != r_end)  
    {
        const std::size_t half = (r_begin + r_end) / 2;
        x->right               = new node_type(pairs[half], x);
        _insert_recursive(x->right, pairs, r_begin, half, half + 1, r_end);
    }
}
template <typename key_t, typename value_t, typename cmp_t>
void bst<key_t, value_t, cmp_t>::_clear( node_type*& t )
    {
        if(t != nullptr)
        {
            _clear( t->left );
            _clear( t->right );
            delete t;
        }
        t = nullptr;
    }

///////////////////////////////////main/////////////////////////////////////////
/*int main()
{
   
    bst< int , double, std::less<int>> t{};
    
    auto x0 = std::pair<int,double>{42,0.123};
    auto x1 = std::pair(55, 2.3);
    auto x2 = std::pair<int,double>{142,0.123};
    
    t.insert(x0);
    t.insert(std::pair<int,double>{4,0.3});
    t.insert(x1);
    t.insert(x2);
    std::cout << "the tree is " << t << std::endl;
    t.find(x0.first);
    t.find(1);
    t.clear();
    return 0;
 }
*/