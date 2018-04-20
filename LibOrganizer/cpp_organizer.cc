#include "inc/cpp_organizer.h"

#include <vector>
#include <set>
#include <algorithm>
#include <chrono>
#include <thread>

#ifdef _WIN32
#include <Windows.h>
#endif

namespace CppOrganizer
{

#ifdef _WIN32
  class ConsoleColorManager {
  public:
    enum Color {
      default = 0,
      fg_blue = FOREGROUND_BLUE,
      fg_green = FOREGROUND_GREEN,
      fg_red = FOREGROUND_RED,
      fg_intensified = FOREGROUND_INTENSITY,
      bg_blue = BACKGROUND_BLUE,
      bg_green = BACKGROUND_GREEN,
      bg_red = BACKGROUND_RED,
      bg_intensified = BACKGROUND_INTENSITY,
    };

    ConsoleColorManager()
    {
      hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
      if (hConsole == NULL/*nullptr*/) {
        return;
      }
      /* Save current attributes */
      if (!GetConsoleScreenBufferInfo(hConsole, &consoleInfo)) return;
      saved_attributes = consoleInfo.wAttributes;
      bInitialized = true;
    }

    ~ConsoleColorManager() {
      /* Restore original attributes */
      if (hConsole)
        SetConsoleTextAttribute(hConsole, saved_attributes);
      //printf("Back to normal");
    }

    bool SetConsoleColor(unsigned short color = Color::default) {
      if (!bInitialized) return false;
      return SetConsoleTextAttribute(hConsole, color) > 0;
    }

    void Default() {
      SetConsoleTextAttribute(hConsole, saved_attributes);
    }

    ConsoleColorManager(const ConsoleColorManager&) = delete;
    ConsoleColorManager& operator=(const ConsoleColorManager&) = delete;
  private:
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;
    HANDLE hConsole;
    bool bInitialized = false;
  };
#else
  class ConsoleColorManager {
  public:
    enum Color {
      default,
      fg_blue,
      fg_green,
      fg_red,
      fg_intensified,
      bg_blue,
      bg_green,
      bg_red,
      bg_intensified,
    };
    bool SetConsoleColor(unsigned short color) {
      return false;
    }
    void Default() { }
  };
#endif
  int ICodeRunnerIdentifier::quest_id = 0;

  ICodeRunnerIdentifier::ICodeRunnerIdentifier(const std::string& _name) :
    id(quest_id++), name(_name) { }

  std::string ICodeRunnerIdentifier::getName() const {
    return name;
  };

  int ICodeRunnerIdentifier::getID() const {
    return id;
  };

  CodeExecuteBenchmark::CodeExecuteBenchmark(std::function<void()> f, const ICodeRunnerIdentifier& iden) : f_(f), iden_(iden) { 
    using clock = std::chrono::high_resolution_clock;
    using duration = std::chrono::duration<double, std::milli>;
    ConsoleColorManager ccm;
    try
    {
      ccm.SetConsoleColor(ConsoleColorManager::fg_intensified | ConsoleColorManager::fg_green);
      printf("\n __START__ ");
      ccm.Default();
      printf("%d: %s is starting execute\n", iden_.getID(), iden_.getName().c_str());
      clock::time_point start = clock::now();
      f_();
      duration elapsed = clock::now() - start;
      ccm.SetConsoleColor(ConsoleColorManager::fg_intensified | ConsoleColorManager::fg_red);
      printf("\n  __END__  ");
      ccm.Default();
      printf("%d: %s completed in", iden_.getID(), iden_.getName().c_str());
      ccm.SetConsoleColor(ConsoleColorManager::fg_intensified | ConsoleColorManager::fg_green);
      printf(" %.2f ", elapsed.count());
      ccm.Default();
      printf("msec\n");
    }
    catch (const std::exception& ex)
    {
      printf("\nCode[%d]:%s failed to execute!\nException message:\n%s", iden_.getID(), iden_.getName().c_str(), ex.what());
    }
    catch (...)
    {
      printf("\nCode[%d]:%s failed to execute!\nUnhandled Exception occured!\n", iden_.getID(), iden_.getName().c_str());
    }
  }
  
  CodeExecuteBenchmark::~CodeExecuteBenchmark() {
  }

  struct CodeRunnerComp {
    using is_transparent = void; // for example with void,
                                 // but could be int or struct CanSearchOnId;
    bool operator()(ICodeRunnerIdentifierPtr const& employee1, ICodeRunnerIdentifierPtr const& employee2) const
    {
      return employee1->getID() < employee2->getID();
    }
    bool operator()(int id, ICodeRunnerIdentifierPtr const& employee) const
    {
      return id < employee->getID();
    }
    bool operator()(ICodeRunnerIdentifierPtr const& employee, int id) const
    {
      return employee->getID() < id;
    }

    bool operator()(const std::string str, ICodeRunnerIdentifierPtr const& employee) const
    {
      return str < employee->getName();
    }
    bool operator()(ICodeRunnerIdentifierPtr const& employee, const std::string str) const
    {
      return employee->getName() < str;
    }
  };

  struct PlaygroundOrganizer::PlaygroundOrganizerPimpl {
    using IPlaygroundObjMap = std::set<ICodeRunnerIdentifierPtr, CodeRunnerComp>;
    IPlaygroundObjMap m_pg_objects;
  };


  PlaygroundOrganizer::PlaygroundOrganizer() : pimpl_ptr(new PlaygroundOrganizerPimpl()) {

  }
  PlaygroundOrganizer::~PlaygroundOrganizer() {
    //pimpl_ptr.reset();
  }

  void PlaygroundOrganizer::Insert(std::initializer_list<ICodeRunnerIdentifierPtr> pg_objects)
  {
    // append given objects to the end of vector
    pimpl_ptr->m_pg_objects.insert(pg_objects);
  }

  void PlaygroundOrganizer::RunWithID(std::initializer_list<int> pg_ids)
  {
    for (auto id : pg_ids) {
      auto it = pimpl_ptr->m_pg_objects.find(id);
      if (it != pimpl_ptr->m_pg_objects.end())
      {
        (*it)->RunCode();
      }
    }
  }

  void PlaygroundOrganizer::RunAll()
  {
    std::for_each(
      pimpl_ptr->m_pg_objects.begin(),
      pimpl_ptr->m_pg_objects.end(),
      [](auto const& pg) { pg->RunCode(); });
  }

  std::string PlaygroundOrganizer::GetQuestionName(int id) {
    auto it = pimpl_ptr->m_pg_objects.find(id);
    return it != pimpl_ptr->m_pg_objects.end() ? (*it)->name : std::string();
  }

  int PlaygroundOrganizer::GetQuestionIDbyName(const std::string& name) {
    auto iter = pimpl_ptr->m_pg_objects.find(name);
    return iter != pimpl_ptr->m_pg_objects.end() ? (*iter)->id : -1; // or return directly first as id
  }
  
  void PlaygroundOrganizer::PrintDetails() {
    std::for_each(pimpl_ptr->m_pg_objects.begin(), pimpl_ptr->m_pg_objects.end(), [](auto const& q) {
      printf("\n%d. Playground id: %d\tname: %s", q->id, q->id, q->name.c_str());
      //std::cout
      //  << q->id << ". " << "Playground id: " << q->id << "\tname: " << q->name << std::endl;
    });
  }

}
