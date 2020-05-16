#include <iostream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <fstream>
#include <cassert>
#include <algorithm>

#include <boost/config.hpp> /* keep it first to prevent nasty warns in MSVC */
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/algorithm.hpp>
#include <boost/functional/hash/hash.hpp>
#include <boost/algorithm/string/find.hpp>

#if 0
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/find.hpp>

using namespace std;
using namespace boost;

int main()
{  
    cout << "* Find Example *" << endl << endl;

    string str1("abc___cde___efg");
    string str2("abc");

    // find "cde" substring
    iterator_range<string::iterator> range=find_first( str1, string("cde") );
    //iterator_range<string::iterator> range=find_first( str1, string("fge") );

    // convert a substring to upper case 
    // note that iterator range can be directly passed to the algorithm
    to_upper( range );
    std::cout << range << std::endl;

    /*cout << "str1 with upper-cased part matching cde: " << str1 << endl;

    // get a head of the string
    iterator_range<string::iterator> head=find_head( str1, 3 );
    cout << "head(3) of the str1: " << string( head.begin(), head.end() ) << endl;

    // get the tail
    head=find_tail( str2, 5 );
    cout << "tail(5) of the str2: " << string( head.begin(), head.end() ) << endl;

    // char processing
    char text[]="hello dolly!";
    iterator_range<char*> crange=find_last(text,"ll");

    // transform the range ( add 1 )
    transform( crange.begin(), crange.end(), crange.begin(), bind2nd( plus<char>(), 1 ) );
    // uppercase the range
    to_upper( crange );

    cout << text << endl;

    cout << endl;*/

    return 0;
}
#else

namespace bmi = boost::multi_index;
namespace bip = boost::interprocess;
namespace bl = boost::lambda;

template<typename T>
using SharedAllocator = bip::allocator<T, bip::managed_mapped_file::segment_manager>;

template<typename CharT>
using SharedBasicString = bip::basic_string<CharT, std::char_traits<CharT>, SharedAllocator<CharT>>;

typedef SharedBasicString<wchar_t> tWSharedString;

template<typename StringT>
using Node = std::pair<StringT, StringT>;

typedef
#if 1
  std::wstring
#else
  tWSharedString
#endif
tNodeString;

typedef Node<tNodeString> tNode;

typedef
#if 1
  std::allocator<tNode>
#else
  SharedAllocator<tNode>
#endif
tNodeAllocator;

struct Tag {
  struct ByKeyPartial;
  struct ByKeyHashed;
  struct ByValue;
  struct ByKeyAndValueComposite;
};

struct partial_string 
{
  partial_string()
    : str()
  {
  }
  
  partial_string(tNodeString const& str)
    : str(str)
  {
  }
  
  partial_string(partial_string const& other)
    : str(other.str)
  {
  }
  
  partial_string(partial_string&& other)
    : str(std::forward<tNodeString>(other.str))
  {
  }
  
  partial_string& operator=(tNodeString const& str) {
    this->str = str;
    return *this;
  }
  
  partial_string& operator=(partial_string const& other) {
    if (this != &other) {
      str = other.str;
    }
    
    return *this;
  }
  
  partial_string& operator=(partial_string&& other) {
    str = std::forward<tNodeString>(other.str);
    return *this;
  }
  
  tNodeString str;
};

namespace boost {
  template <>
  struct hash<::partial_string>
    : public std::unary_function<::partial_string, std::size_t>
  {
    std::size_t operator()(::partial_string const& v) const {
      return boost::hash_value(v.str);
    }
  };
} /// end namespace boost

struct partial_str_less
{
  typedef bool result_type;
  
  bool operator()(const tNodeString& x,const tNodeString& y)const
  {
    return x<y;
  }

  bool operator()(const tNodeString& x,const partial_string& y)const
  {
    auto range = boost::algorithm::find_first(x, y.str);
    return range.empty();
    
    /*
   //return x.substr(0,y.str.size())<y.str;
    size_t const p = x.find(y.str);
    //return p != tNodeString::npos;
    
    if (p == tNodeString::npos) {
      return false;
    }
    
    return x.substr(p,y.str.size())<y.str;*/
  }

  bool operator()(const partial_string& x,const tNodeString& y)const
  {
    auto range = boost::algorithm::find_first(y, x.str);
    return range.empty();
    /*
    //return x.str<y.substr(0,x.str.size());
    
    size_t const p = y.find(x.str);
    return p != tNodeString::npos;
    /*if (p == tNodeString::npos) {
      return false;
    }
    
    return x.str<y.substr(p,x.str.size());*/
  }
  
  /*bool operator()(const partial_string& x,const partial_string& y)const
  {
    return x.str < y.str;
  }*/
};

typedef bmi::composite_key<
          tNode
          , bmi::member<tNode, tNodeString, &tNode::first>
          , bmi::member<tNode, tNodeString, &tNode::second>
        > tKeyAndValueCompositeKey;

typedef boost::multi_index_container<
  tNode,
  bmi::indexed_by<
    //bmi::hashed_unique
    //bmi::ordered_unique
    bmi::ordered_non_unique
    <
      bmi::tag<Tag::ByKeyPartial>
      , bmi::member<tNode, tNodeString, &tNode::first>
      //, partial_str_less
    >
    , bmi::hashed_non_unique<
      bmi::tag<Tag::ByKeyHashed>
      , bmi::member<tNode, tNodeString, &tNode::first>
    >
    , bmi::ordered_non_unique<
        bmi::tag<Tag::ByKeyAndValueComposite>
        , tKeyAndValueCompositeKey
        /*, bmi::composite_key_compare<
          partial_str_less
          , partial_str_less
        >*/
    >
    , 
    //bmi::hashed_unique
    bmi::ordered_non_unique
    <
      bmi::tag<Tag::ByValue>
      , bmi::member<tNode, tNodeString, &tNode::second>
      //, partial_str_less
    >
  >
  , tNodeAllocator
> tKeyValueMap;

struct my_custom_comp
{
    /*bool operator()( 
        const boost::tuple<tNodeString>& x,
        const boost::multi_index::composite_key_result<tKeyAndValueCompositeKey>& y ) const
    {
        return false; // should return something instead of false
    }
    bool operator()( 
        const boost::multi_index::composite_key_result<tKeyAndValueCompositeKey>& y,
        const boost::tuple<tNodeString>& x ) const
    {
        return false; // should return something instead of false
    }
    
    bool operator()( 
        const boost::tuple<tNodeString, tNodeString>& x,
        const boost::multi_index::composite_key_result<tKeyAndValueCompositeKey>& y ) const
    {
        return false; // should return something instead of false
    }
    bool operator()( 
        const boost::multi_index::composite_key_result<tKeyAndValueCompositeKey>& y,
        const boost::tuple<tNodeString, tNodeString>& x ) const
    {
        return false; // should return something instead of false
    }
    
    bool operator()( 
        const boost::tuple<tNodeString, partial_string>& x,
        const boost::multi_index::composite_key_result<tKeyAndValueCompositeKey>& y ) const
    {
        return false; // should return something instead of false
    }
    bool operator()( 
        const boost::multi_index::composite_key_result<tKeyAndValueCompositeKey>& y,
        const boost::tuple<tNodeString, partial_string>& x ) const
    {
        return false; // should return something instead of false
    }
    */

    bool operator()( 
        const boost::tuple<partial_string>& x,
        const boost::multi_index::composite_key_result<tKeyAndValueCompositeKey>& y ) const
    {
      std::wcout << "ps->v" << '[' << y.value.first << ':' << y.value.second << ']' << std::endl;
        //return partial_str_less()(x.get<0>(), y.value.first);
      /*return 
            y.value.first.find(x.get<0>().str) == tNodeString::npos 
            || y.value.second.find(x.get<0>().str) == tNodeString::npos
      ;*/
      
      auto fp = y.value.first.find(x.get<0>().str);
      auto sp = y.value.second.find(x.get<0>().str);
      return //fp != tNodeString::npos ||
      sp != tNodeString::npos;
    }
    
    bool operator()(
        const boost::multi_index::composite_key_result<tKeyAndValueCompositeKey>& y,
        const boost::tuple<partial_string>& x ) const
    {
      std::wcout << "v->ps" << '[' << y.value.first << ':' << y.value.second << ']' << std::endl;
        //return partial_str_less()(y.value.second, x.get<0>());
      
      
      /*return 
            y.value.first.find(x.get<0>().str) != tNodeString::npos 
            || y.value.second.find(x.get<0>().str) != tNodeString::npos
      ;*/
      
      auto fp = y.value.first.find(x.get<0>().str);
      auto sp = y.value.second.find(x.get<0>().str);
      return fp != tNodeString::npos;// || sp != tNodeString::npos;
    }
};


int main() {
  tKeyValueMap mapping;
  
  mapping.insert(tNode(L"Anton", L"Poturaeiv"));
  mapping.insert(tNode(L"Anton", L"Poturaev"));
  mapping.insert(tNode(L"Antonina", L"Sernitskaya"));
  
  mapping.insert(tNode(L"Alexander", L"Ismailov"));
  mapping.insert(tNode(L"Maksim", L"Baderin"));
  mapping.insert(tNode(L"Dmytro", L"Magilin"));
  mapping.insert(tNode(L"Ievgenii", L"Svitlychnyi"));
  mapping.insert(tNode(L"Svetlana", L"Lalym"));
  mapping.insert(tNode(L"Viktor", L"Latsyn"));
  
  //tNodeString const pattern(L"Anto");
  
  typedef tKeyValueMap::index<Tag::ByKeyAndValueComposite>::type tByKeyAndValueCompositeView;
  tByKeyAndValueCompositeView const& keyAndValueCompositeView = mapping.get<Tag::ByKeyAndValueComposite>();
  
  auto f = keyAndValueCompositeView.lower_bound(boost::make_tuple(partial_string(L"nto")), my_custom_comp());
  auto l = keyAndValueCompositeView.upper_bound(boost::make_tuple(partial_string(L"nto")), my_custom_comp());
  
  if (f != l) {
    while (f != l) {
      std::wcout << f->first << L'\t' << f->second << std::endl;
      ++f;
    }
  } else {
    ;
  }
  
  /*auto range_kvc = keyAndValueCompositeView.equal_range(
    boost::make_tuple(
      //tNodeString(L"nt")
      //partial_string(L"")
      //, 
      //tNodeString()
      partial_string(L"nto")
    )
    , my_custom_comp()
  );
  
  if (range_kvc.first != range_kvc.second) {
    while (range_kvc.first != range_kvc.second) {
      std::wcout << range_kvc.first->first << L'\t' << range_kvc.first->second << std::endl;
      ++range_kvc.first;
    }
  } else {
    ;
  }*/
  
  /*{
    typedef tKeyValueMap::index<Tag::ByKeyHashed>::type tByKeyHashedView;
    tByKeyHashedView const& keyHashedView = mapping.get<Tag::ByKeyHashed>();
    
    auto range_kh = keyHashedView.equal_range(pattern);
    
    if (range_kh.first != range_kh.second) {
      while (range_kh.first != range_kh.second) {
        std::wcout << range_kh.first->second << std::endl;
        ++range_kh.first;
      }
    } else {
      typedef tKeyValueMap::index<Tag::ByKeyPartial>::type tByKeyPartialView;
      tByKeyPartialView const& keyPartialView = mapping.get<Tag::ByKeyPartial>();
      
      auto range_kp = keyPartialView.equal_range(partial_string(pattern));
    
      if (range_kp.first != range_kp.second) {
        while (range_kp.first != range_kp.second) {
          std::wcout << range_kp.first->second << std::endl;
          ++range_kp.first;
        }
      }
    }*/
    
    /*auto first_kh = keyHashedView.lower_bound(pattern);
    auto last_kh = keyHashedView.upper_bound(pattern);
    
    if (first_kh != last_kh) {
      while (first_kh != last_kh) {
        std::wcout << first_kh->second << std::endl;
        ++first_kh;
      }
    } else {
      if (first_kh != keyHashedView.end()) {
        std::wcout << first_kh->second << std::endl;
      }
    }*/
  //}
  
  
  
#if 0
  //auto found = keyView.find(L"Anton");
  /*auto found = keyView.lower_bound(L"Ant", comp_substr());
  
  assert(found != keyView.end());
  
  std::wcout << found->second << std::endl;*/
  
  //auto range = keyView.equal_range(L"Ant", comp_substr());
  //auto range = keyView.equal_range(L"ton", keyView.hash_function(), comp_substr());
  /*auto range = keyView.equal_range(L"ton", comp_substr());
  
  while (range.first != range.second) {
     std::wcout << range.first->second << std::endl;
     ++range.first;
  }*/
  
#if 0
  auto first = keyView.lower_bound(L"ton"/*, comp_substr_lb()*/);
  auto const last = keyView.upper_bound(L"ton"/*, comp_substr_ub()*/);
  
  while (first != last) {
     std::wcout << first->second << std::endl;
     ++first;
  }
#endif



  auto range = keyView.equal_range(partial_string(L"Anton"));
  while (range.first != range.second) {
     std::wcout << range.first->second << std::endl;
     ++range.first;
  }
#endif
  return 0;
}
#endif