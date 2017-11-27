#ifndef _CPP_Playground_ORGANIZER_HPP_
#define _CPP_Playground_ORGANIZER_HPP_

#include <iostream>
#include <sstream>      // std::ostringstream
#include <functional>
#include <map>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>

namespace CppOrganizer
{
  struct ICodeRunnerIdentifier
  {
    ICodeRunnerIdentifier() : id(quest_id++) {}
    int id;
    std::string name;
    std::function<void()> run_method;
  private:
    static int quest_id;
  };
  int ICodeRunnerIdentifier::quest_id = 0;
  typedef std::shared_ptr<ICodeRunnerIdentifier> ICodeRunnerIdentifierPtr;
  
  typedef std::map<int, ICodeRunnerIdentifierPtr> IPlaygroundObjMap;

  extern std::vector<ICodeRunnerIdentifierPtr> _default_objects;
 
  template <class T>
  class CodeRunnerHelper : public ICodeRunnerIdentifier
  {
  public: 
    CodeRunnerHelper() = delete; // delete default ctor, only accepted named 
    explicit CodeRunnerHelper(const std::string& testName)
    {
      name = testName;
      run_method = std::bind(&CodeRunnerHelper::Run, this);
    }

    void Run() {
      typedef std::chrono::high_resolution_clock clock;
      typedef std::chrono::duration<double, std::micro> duration;

      clock::time_point start = clock::now();
      static_cast<T *>(this)->RunTest();
      duration elapsed = clock::now() - start;
      printf("\nTest[%d]:%s completed in %f us", id, name.c_str(), elapsed.count());
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

    void Insert(const std::vector<ICodeRunnerIdentifierPtr>& pg_objects = _default_objects)
    {
      for (auto q : pg_objects)
      {
        pg_map.insert(std::make_pair(q->id, q));
      }
      /*
      // populate question map.
      std::for_each(
        pg_objects.begin(),
        pg_objects.end(),
        [&](auto q) {
          pg_map.insert(std::make_pair(q->id, q));
        });
      */
    }

    void RunWithID(const std::vector<int>& pg_objects)
    {
      for (auto id : pg_objects) {
        if (pg_map.find(id) == pg_map.end()) continue;
        pg_map[id]->run_method();
      }
    }

    void RunAll()
    {
      for (auto pg : pg_map) {
        pg.second->run_method();
      }
    }

    std::vector<IPlaygroundObjMap::key_type> All() const {
      std::vector<IPlaygroundObjMap::key_type> all_keys;
      for (auto const& keys : pg_map)
        all_keys.push_back(keys.first);
      return std::move(all_keys);
    }

    std::string GetQuestionName(int id) {
      return pg_map[id]->name;
    }

    int GetQuestionID(const std::string& name) {
      auto iter = std::find_if(pg_map.begin(), pg_map.end(), [name](auto q)-> bool { return q.second->name == name; });
      return iter != pg_map.end() ? iter->second->id : -1; // or return directly first as id
    }

    void PrintDetails(std::ostream& out = std::cout) {
      //std::ostringstream o_str;
      std::for_each(pg_map.begin(), pg_map.end(), [&out](auto q) {
        out
          << q.second->id << ". " << "Playground id: " << q.second->id << "\tname: " << q.second->name << std::endl;
          //<< "TesterFunction: " << q.second.run_method.target_type().name() << std::endl;
      });
    }

  private:
    IPlaygroundObjMap pg_map;
  };

}

#endif // _CPP_Playground_ORGANIZER_HPP_
