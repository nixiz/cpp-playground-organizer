#ifndef _CPP_Playground_ORGANIZER_HPP_
#define _CPP_Playground_ORGANIZER_HPP_

#include <iostream>
#include <sstream>      // std::ostringstream
#include <functional>
#include <map>
#include <vector>
#include <algorithm>

namespace CppOrganizer
{

  struct IPlaygroundObj
  {
    int id;
    std::string name;
    std::function<void()> run_method;
  };
  typedef std::map<int, IPlaygroundObj> IPlaygroundObjMap;

  extern std::vector<IPlaygroundObj> _default_objects;

  static int quest_id = 0;

  template <class T>
  class PlaygroundHelper : public IPlaygroundObj
  {
  public: 
    PlaygroundHelper() = delete; // delete default ctor, only accepted named 
    explicit PlaygroundHelper(const std::string& testName)
    {
      id = quest_id++;
      name = testName;
      run_method = std::bind(&PlaygroundHelper::Run, this);
    }

    void Run() {
      static_cast<T *>(this)->RunTest();
    }

  protected:
    void RunTest() {
      std::cout << "Test is not implemented yet!" << std::endl;
    }
  };

  class PlaygroundOrganizer
  {
  public:
    PlaygroundOrganizer() = default;
    ~PlaygroundOrganizer() = default;

    void Initialize(const std::vector<IPlaygroundObj>& pg_objects = _default_objects)
    {
      //static int _id = 0;
      // populate question map.
      std::for_each(
        const_cast<std::vector<IPlaygroundObj>&>(pg_objects).begin(),
        const_cast<std::vector<IPlaygroundObj>&>(pg_objects).end(),
        [&](auto q) {
        pg_map.insert(std::make_pair(q.id, q));
      });
    }

    void Run(const std::vector<int>& pg_objects)
    {
      for (auto id : pg_objects) {
        if (pg_map.find(id) == pg_map.end()) continue;
        pg_map[id].run_method();
      }
    }

    std::vector<IPlaygroundObjMap::key_type> All() const {
      std::vector<IPlaygroundObjMap::key_type> all_keys;
      for (auto const& keys : pg_map)
        all_keys.push_back(keys.first);
      return std::move(all_keys);
    }

    std::string GetQuestionName(int id) {
      return pg_map[id].name;
    }

    int GetQuestionID(const std::string& name) {
      auto iter = std::find_if(pg_map.begin(), pg_map.end(), [name](auto q)-> bool { return q.second.name == name; });
      return iter != pg_map.end() ? iter->second.id : -1; // or return directly first as id
    }

    void PrintDetails(std::ostream& out = std::cout) {
      //std::ostringstream o_str;
      std::for_each(pg_map.begin(), pg_map.end(), [&out](auto q) {
        out
          << q.second.id << ". " << "Playground id: " << q.second.id << "\tname: " << q.second.name << std::endl;
          //<< "TesterFunction: " << q.second.run_method.target_type().name() << std::endl;
      });
    }

  private:
    IPlaygroundObjMap pg_map;
  };

}

#endif // _CPP_Playground_ORGANIZER_HPP_
