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
#include <cstddef>  /* std::ptrdiff_t */
#include <iterator> /* std::forward_iterator_tag */
#include <iostream>
#include <vector>


template <typename T>

struct _node {
    T value;
    std::unique_ptr<_node> left;
    std::unique_ptr<_node> right;
    _node* parent;

    explicit _node(const T& x, _node* y) noexcept : value{x}, parent{y} {}  // can be explicit
    explicit _node(T&& x, _node* y) noexcept: value{std::move(x)}, parent{y} {}

    explicit _node(const std::unique_ptr<_node>& x, _node* y) noexcept
        : value{x->value}, parent{y} {
        if (x->left)
            left = std::make_unique<_node>(x->left, this);
        if (x->right)
            right = std::make_unique<_node>(x->right, this);
    };

};  // end of class _node


// helpers functions

template <typename T>
T* _left_most(T* x) 
{
    if (!x)
        return x;
    while (x->left)
        x = x->left.get();
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
        if (parent->left.get() == current)
            break;
        current = parent;
        parent  = parent->parent;
    }
    return parent;
}


template <typename node_t, typename deref_t>
// template < a pointer to node, a pair of key and value >
class _iterator {
public:  
    node_t * current;
    
    using value_type        = deref_t;
    using reference         = value_type&;
    using pointer           = value_type*;
  
    _iterator(node_t* x) : current{x} {}   // constructor, not explicit, there an implicit conversion to _iterator in bst class
    
    _iterator( std::unique_ptr<node_t>  x) : current{std::move(x)} {}
    
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

  
    friend bool operator==(const _iterator& x, const _iterator& y) 
    {        return x.current == y.current;    }
  
    friend bool operator!=(const _iterator& x, const _iterator& y) 
    {        return !(x == y);      }

};  //end of class _iterator


template <typename key_t, typename value_t, typename cmp_t = std::less<key_t>>
class bst {


public:

    using pair_type = std::pair<const key_t, value_t>;
    using node_type = _node<pair_type>;
    using iterator       = _iterator<node_type, pair_type>;
    using const_iterator = _iterator<node_type, const pair_type>;

/////////////////////////////constructor - destructor /////////////////////////////
    bst() noexcept  = default;
    bst(bst&&) noexcept = default;  // move semantique doesn't work without noexcept
    bst(const bst& x) : op{x.op} 
    {
        if (x.root)
            root = std::make_unique<node_type>(x.root, nullptr);
    }

    ~bst() noexcept = default;

////////////////////////////////////////////////operators////////////////////////////////////////////    
    
    value_t& operator[](const key_t& x) { return _find_or_insert(x); }
    value_t& operator[](key_t&& x) { return _find_or_insert(std::move(x)); }


//////////////////////////////////////////public functions///////////////////////////////
    // the insert functions as _insert return a pointer to node which will be converted implicitly to an _iterator, and a bool wich says if the function inserted x or no.
    std::pair<iterator, bool> insert(const pair_type& x) { return _insert(x,root,nullptr); }
    std::pair<iterator, bool> insert(pair_type&& x) {return _insert(std::move(x),root,nullptr); }

    template <typename... Types>   
    std::pair<iterator, bool> emplace(Types&&... args) { return insert({std::forward<Types>(args)...});}

    iterator find(const key_t& x) noexcept { return _find(x); }
    const_iterator find(const key_t& x) const noexcept { return _find(x); }

    iterator begin() noexcept { return _begin(); }
    const_iterator begin() const noexcept { return _begin(); }
    const_iterator cbegin() const noexcept { return begin(); }

    iterator end() noexcept { return nullptr; }
    const_iterator end() const noexcept { return nullptr; }
    const_iterator cend() const noexcept { return end(); }
    
    void balance(); 

    void clear() noexcept { root.reset(); }  //Destroys the object currently managed by the unique_ptr (if any) and takes ownership of p
    
private:

    cmp_t op;
    std::unique_ptr<node_type> root;   
    
   //////////////////////////////private functions///////////////////////////////////// 
   

    std::pair< iterator , bool > _insert(pair_type&& x, std::unique_ptr<node_type> & p, node_type* par); 
    
    node_type* _begin() const noexcept { return _left_most(root); }
    
    template <typename T> value_t & _find_or_insert(T&& x) 
    {
        auto it = find(x);
        if (it != end())
            return it->second;
        return emplace(std::forward<T>(x), value_t{}).first->second;
    }


    //template <typename key_t, typename value_t, typename cmp_t>
    node_type * _find(const key_t& x) const noexcept;
    void _insert_recursive( node_type* x,
                            const std::vector<typename bst<key_t, value_t, cmp_t>::pair_type>& pairs,
                            const std::size_t l_begin,
                            const std::size_t l_end,
                            const std::size_t r_begin,
                            const std::size_t r_end); 

    friend std::ostream& operator<<(std::ostream& os, const bst& tree) 
    {
        for (const auto& x : tree)
            os << "(" << x.first << "," << x.second << ")" << " ";
        return os;
    }

   /* void display() const
    {
        auto tmp = root.get();
        while (tmp) 
        {   
            std::cout << tmp << ' '
            
    }
    */
};//end of class bst


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
bst<key_t, value_t, cmp_t>::_insert(pair_type&& x, std::unique_ptr<node_type>& p, node_type* par)  
{  
    if( !p )
    {
        p = std::make_unique<node_type>(std::forward<pair_type>(x), par);
        return { p.get() , true };
    }

    else
    {
        auto tmp = p.get();
        const auto& k = tmp->value.first;
           
        if      ( op( x.first , k ) )   return _insert(std::move(x), (tmp->left), p.get());
        else if ( op( k , x.first ) )   return _insert(std::move(x), (tmp->left), p.get());
        else                            return {nullptr, false};
    }
}


template <typename key_t, typename value_t, typename cmp_t>
typename bst<key_t, value_t, cmp_t>::node_type*
bst<key_t, value_t, cmp_t>::_find(const key_t& x) const noexcept 
{
    auto tmp = root.get();
    while (tmp) 
    {
        const auto& k = tmp->value.first;
        if (op(x, k))
            tmp = tmp->left.get();
        else if (op(k, x))
            tmp = tmp->right.get();
        else
            break;
    }
    return tmp;
}


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
    root = std::make_unique<node_type>(pairs[half], nullptr);
    

    _insert_recursive(root.get(), pairs, 0, half, half + 1, pairs.size());

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
        x->left                = std::make_unique<node_type>(pairs[half], x);
        _insert_recursive(x->left.get(), pairs, l_begin, half, half + 1, l_end);
    }

  //  if (r_begin != r_end)  
    {
        const std::size_t half = (r_begin + r_end) / 2;
        x->right               = std::make_unique<node_type>(pairs[half], x);
        _insert_recursive(x->right.get(), pairs, r_begin, half, half + 1, r_end);
    }
}


