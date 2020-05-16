// CppDB.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#if 0

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
    iterator_range<char*> crange=find_last(text,"l");

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

typedef SharedBasicString<char> tSharedString;

template<typename StringT>
using Node = std::pair<StringT, StringT>;

typedef
#if 1
  std::string
#else
  tSharedString
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
    bool operator()( 
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
    

    bool operator()( 
        const boost::tuple<partial_string>& x,
        const boost::multi_index::composite_key_result<tKeyAndValueCompositeKey>& y ) const
    {
      //std::cout << "ps->v" << '[' << y.value.first << ':' << y.value.second << ']' << std::endl;
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
      //std::cout << "v->ps" << '[' << y.value.first << ':' << y.value.second << ']' << std::endl;
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
  /*
  tKeyValueMap mapping;
  
  mapping.insert(tNode("Anton", "Poturaeiv"));
  mapping.insert(tNode("Anton", "Poturaev"));
  mapping.insert(tNode("Antonina", "Sernitskaya"));
  
  mapping.insert(tNode("Alexander", "Ismailov"));
  mapping.insert(tNode("Maksim", "Baderin"));
  mapping.insert(tNode("Dmytro", "Magilin"));
  mapping.insert(tNode("Ievgenii", "Svitlychnyi"));
  mapping.insert(tNode("Svetlana", "Lalym"));
  mapping.insert(tNode("Viktor", "Latsyn"));

  {
    /// Print

    typedef tKeyValueMap::index<Tag::ByKeyAndValueComposite>::type tByKeyAndValueCompositeView;
    tByKeyAndValueCompositeView const& keyAndValueCompositeView = mapping.get<Tag::ByKeyAndValueComposite>();

    for (auto const& current : keyAndValueCompositeView) {
      std::cout << '[' << current.first << ':' << current.second << ']' << std::endl;
    }

    std::cout << "----------------------------------" << std::endl;
  }
  
  //tNodeString const pattern("Anto");
  //std::lower_bound();
  typedef tKeyValueMap::index<Tag::ByKeyAndValueComposite>::type tByKeyAndValueCompositeView;
  tByKeyAndValueCompositeView const& keyAndValueCompositeView = mapping.get<Tag::ByKeyAndValueComposite>();

  for (
    auto current = std::find_if(keyAndValueCompositeView.begin(), keyAndValueCompositeView.end()
      , [](tNode const& node) {
            return node.first.find("t") != tNodeString::npos || node.second.find("t") != tNodeString::npos;
        }
    );
    current != keyAndValueCompositeView.end();
    current = std::find_if(++current, keyAndValueCompositeView.end()
      , [](tNode const& node) {
            return node.first.find("t") != tNodeString::npos || node.second.find("t") != tNodeString::npos;
        }
    )
      ) {
    std::cout << '[' << current->first << ':' << current->second << ']' << std::endl;
  }

  /*auto f = keyAndValueCompositeView.lower_bound(boost::make_tuple(partial_string("t")), 
    [](
      const boost::multi_index::composite_key_result<tKeyAndValueCompositeKey>& node,
      const boost::tuple<partial_string>& partialPatternTuple
      ) {
          auto fp = node.value.first.find(partialPatternTuple.get<0>().str);
          auto sp = node.value.second.find(partialPatternTuple.get<0>().str);
          
          return fp == tNodeString::npos && sp == tNodeString::npos;
    }
  );

  //std::cout << f->first << '\t' << f->second << std::endl;

  auto l = keyAndValueCompositeView.upper_bound(boost::make_tuple(partial_string("t")),
    [](
      const boost::tuple<partial_string>& partialPatternTuple,
      const boost::multi_index::composite_key_result<tKeyAndValueCompositeKey>& node
      ) {
          auto fp = node.value.first.find(partialPatternTuple.get<0>().str);
          auto sp = node.value.second.find(partialPatternTuple.get<0>().str);
          
          return fp == tNodeString::npos && sp == tNodeString::npos;
    }
  );
  
  if (f != l) {
    while (f != l) {
      std::cout << f->first << '\t' << f->second << std::endl;
      ++f;
    }
  } else {
    ;
  }*/
  
  /*auto range_kvc = keyAndValueCompositeView.equal_range(
    boost::make_tuple(
      //tNodeString("nt")
      //partial_string("")
      //, 
      //tNodeString()
      partial_string("nto")
    )
    , my_custom_comp()
  );
  
  if (range_kvc.first != range_kvc.second) {
    while (range_kvc.first != range_kvc.second) {
      std::cout << range_kvc.first->first << L'\t' << range_kvc.first->second << std::endl;
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
        std::cout << range_kh.first->second << std::endl;
        ++range_kh.first;
      }
    } else {
      typedef tKeyValueMap::index<Tag::ByKeyPartial>::type tByKeyPartialView;
      tByKeyPartialView const& keyPartialView = mapping.get<Tag::ByKeyPartial>();
      
      auto range_kp = keyPartialView.equal_range(partial_string(pattern));
    
      if (range_kp.first != range_kp.second) {
        while (range_kp.first != range_kp.second) {
          std::cout << range_kp.first->second << std::endl;
          ++range_kp.first;
        }
      }
    }*/
    
    /*auto first_kh = keyHashedView.lower_bound(pattern);
    auto last_kh = keyHashedView.upper_bound(pattern);
    
    if (first_kh != last_kh) {
      while (first_kh != last_kh) {
        std::cout << first_kh->second << std::endl;
        ++first_kh;
      }
    } else {
      if (first_kh != keyHashedView.end()) {
        std::cout << first_kh->second << std::endl;
      }
    }*/
  //}
  
  
  
#if 0
  //auto found = keyView.find("Anton");
  /*auto found = keyView.lower_bound("Ant", comp_substr());
  
  assert(found != keyView.end());
  
  std::cout << found->second << std::endl;*/
  
  //auto range = keyView.equal_range("Ant", comp_substr());
  //auto range = keyView.equal_range("ton", keyView.hash_function(), comp_substr());
  /*auto range = keyView.equal_range("ton", comp_substr());
  
  while (range.first != range.second) {
     std::cout << range.first->second << std::endl;
     ++range.first;
  }*/
  
#if 0
  auto first = keyView.lower_bound("ton"/*, comp_substr_lb()*/);
  auto const last = keyView.upper_bound("ton"/*, comp_substr_ub()*/);
  
  while (first != last) {
     std::cout << first->second << std::endl;
     ++first;
  }
#endif



  auto range = keyView.equal_range(partial_string("Anton"));
  while (range.first != range.second) {
     std::cout << range.first->second << std::endl;
     ++range.first;
  }
#endif
  ::getchar();
  return 0;
}
#endif

#else

#include <iostream>
#include <string>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/container/string.hpp>

#include <CppDb/Manager.hpp>

namespace {
  
namespace CppADB = CppAbstractDataBase;

typedef CppADB::Allocator<char> tCharAllocator;
typedef boost::container::basic_string<char, std::char_traits<char>, tCharAllocator> tString;

struct Data {
  typedef CppADB::Allocator<Data> tAllocator;

  /*Data(tAllocator const& allocator)
    : 
  {
  }*/

  int a;
  tString s;
};

typedef boost::multi_index_container<
  Data
  , boost::multi_index::indexed_by<
      boost::multi_index::ordered_unique<
        boost::multi_index::member<
          Data, int, &Data::a
        >
      >
      , boost::multi_index::ordered_unique<
          boost::multi_index::member<
            Data, tString , &Data::s
          >
      >
  >
  , Data::tAllocator
> tStore;

struct Node {
  wchar_t character;
  std::wstring translate;
};

CppADB::FileSystemStructure _MakeFsStructure() {
  CppADB::FileSystemStructure fss = { 
#if defined(__linux__)
    "/home/apoturaiev/Документы/work/tests_db/"
#elif defined(_WIN32)
    //"../"
    "E:\\projects\\CppDB\\"
#else
# error undefined OS
#endif
    , "cpp_a_db_test" };
  //fss.rootDir.lexically_normal();
  return fss;
}

} /// end unnamed namespace

#include <fstream>

#include <boost/lexical_cast.hpp>

tString _ToStr(size_t i, CppADB::Details::tSegmentManager* segmentManager) {
  tString result("__str_value[", tString::allocator_type(segmentManager));
  
  typedef std::basic_string<tString::value_type> tCnvString;
  auto const cnv = [i]() -> tCnvString {
    try {
      return boost::lexical_cast<tCnvString>(i);
    }
    catch (boost::bad_lexical_cast const&) {
      return "<<BAD_VALUE_CAST>>";
    }
  };

  auto append = [&result](tCnvString&& str) mutable {
    result.append(str.begin(), str.end());
  };

  append(cnv());

  result += ']';

  return result;
}

int main() {
  try {
#if 0
   CppADB::Details::ConcreteDataBase<tStore> cdb(_MakeFsStructure(), CppADB::tControllInformationOptional());
   cdb.GetBitCount();

#else
    auto& i = CppADB::Manager::Instance();
    
    if (CppADB::tIDataBasePtr db = CppADB::Manager::Instance().AcquireDataBase<tStore>(_MakeFsStructure())) {
      if (CppADB::tSessionPtr session = db->CreateSession()) {

        session->Transaction([](CppADB::Session::tContextWeak&& ctxWeak) {
          if (CppADB::Session::tContextPtr ctx = ctxWeak.lock()) {
            if (CppADB::tAbstractContainerPtr abstractContainer = ctx->GetAbstractContainer()) {
              if (tStore* store = static_cast<tStore*>(abstractContainer)) {
                auto& byDataIdView = store->get<0>();

                if (byDataIdView.empty()) {
                  for (size_t i = 0; i < 1000; ++i) {
                    byDataIdView.insert({ static_cast<int>(i), _ToStr(i, ctx->GetSegmentManager()) });
                  }
                }
              }
            }
          }
        });

        session->Transaction([](CppADB::Session::tContextWeak&& ctxWeak) {
          if (CppADB::Session::tContextPtr ctx = ctxWeak.lock()) {
            if (CppADB::tAbstractContainerPtr abstractContainer = ctx->GetAbstractContainer()) {
              if (tStore* store = static_cast<tStore*>(abstractContainer)) {
                auto& byDataIdView = store->get<0>();

                if (!byDataIdView.empty()) {
                  size_t const start = std::prev(byDataIdView.end())->a + 1;
                  for (size_t i = start; i < start * 1000; ++i) {
                    byDataIdView.insert({ static_cast<int>(i), _ToStr(i, ctx->GetSegmentManager()) });
                  }
                }
              }
            }
          }
        });

        session->Transaction([](CppADB::Session::tContextWeak&& ctxWeak) {
          if (CppADB::Session::tContextPtr ctx = ctxWeak.lock()) {
            if (CppADB::tAbstractContainerPtr abstractContainer = ctx->GetAbstractContainer()) {
              if (tStore* store = static_cast<tStore*>(abstractContainer)) {
                auto& byDataIdView = store->get<0>();
                std::string buffer;
                buffer.reserve(1024 * 1024);

                for (auto const& i : byDataIdView) {
                  buffer.append(std::to_string(i.a) + ": ");
                  buffer.append(i.s.begin(), i.s.end());
                  buffer.append(1, '\n');
                }

                std::cout.rdbuf()->sputn(buffer.data(), buffer.size());
              }
            }
          }
        });
      }
      
      CppADB::Manager::Instance().ReleaseDataBase(db->GetDbId());
    }
#endif
  } catch(...) {
    int const stop = 0;
  }


  ::getchar();
  return 0;
}

#endif
