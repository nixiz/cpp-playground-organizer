#ifndef _CPP_Playground_ORGANIZER_HPP_
#define _CPP_Playground_ORGANIZER_HPP_

#include <memory>
#include <vector>
#include <set>
#include <algorithm>
#include <chrono>
#include <thread>
#include <functional>

#ifndef COMPILE_WITH_UNIMPLEMENTED_RUNNER
#define COMPILE_WITH_UNIMPLEMENTED_RUNNER 1
#endif // !COMPILE_WITH_UNIMPLEMENTED_RUNNER

#define START_ORGANIZER_MAIN_WITH_OBJS(...) \
int main(int argc, char *argv[]) { \
  CppOrganizer::PlaygroundOrganizer po; \
  po.Insert(__VA_ARGS__); \
  po.PrintDetails(); \
  po.RunAll(); 
#define PAUSE_ON_END system("pause");
#define END_ORGANIZER_MAIN \
  return EXIT_SUCCESS; \
}

#define CREATE_ELEMENT(ClassName)       \
class ClassName :                       \
  public CppOrganizer::CodeRunnerHelper<ClassName> {  \
public:                                 \
  void Run();                           \
}

#define ELEMENT_CODE(ClassName) void ClassName::Run()

#define ADD_ELEMENT(ClassName, ...) \
std::make_shared<ClassName>(__VA_ARGS__)

#define CREATE_ELEMENT_WITH_CODE(ClassName)  \
CREATE_ELEMENT(ClassName);              \
ELEMENT_CODE(ClassName)


namespace colorconsole {
#ifdef _WIN32
#include <Windows.h>
#endif

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

}

namespace CppOrganizer
{
  using colorconsole::ConsoleColorManager;
  class PlaygroundOrganizer;

  class ICodeRunnerIdentifier {
    friend class PlaygroundOrganizer;
  public:
    explicit ICodeRunnerIdentifier(const std::string& _name) :
      id(quest_id++), name(_name) { }

    std::string getName() const {
      return name;
    }

    int getID() const {
      return id;
    }

  private:
    virtual void RunCode() {};
    const std::string name;
    const int id;
    static int quest_id;
  };
  int ICodeRunnerIdentifier::quest_id = 0;
  typedef std::shared_ptr<ICodeRunnerIdentifier> ICodeRunnerIdentifierPtr;

  class CodeExecuteBenchmark {
  public:
    explicit CodeExecuteBenchmark(std::function<void()> f, const ICodeRunnerIdentifier& iden) : f_(f), iden_(iden) {
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
    ~CodeExecuteBenchmark() {
    }
  private:
    std::function<void()> f_;
    const ICodeRunnerIdentifier& iden_;
  };

  template <class T>
  class CodeRunnerHelper : public ICodeRunnerIdentifier {
  public:
    CodeRunnerHelper(const std::string& testName = typeid(T).name()) : ICodeRunnerIdentifier(testName) { }
    virtual ~CodeRunnerHelper() {};

    void RunCode() override {
      CodeExecuteBenchmark{ std::bind(&T::Run, static_cast<T *>(this)), *this };
    }

  protected:
#if COMPILE_WITH_UNIMPLEMENTED_RUNNER == 1
    void Run() {
      printf("\n%s\n%s::Run() should be implemented!\n", PrintWarn, typeid(T).name());;
    }
#endif
  };

  struct CodeRunnerComp {
    using is_transparent = void; // for example with void,
                                 // but could be int or struct CanSearchOnId;
    bool operator()(ICodeRunnerIdentifierPtr const& lhs, ICodeRunnerIdentifierPtr const& rhs) const
    {
      return lhs->getID() < rhs->getID();
    }
    bool operator()(int id, ICodeRunnerIdentifierPtr const& rhs) const
    {
      return id < rhs->getID();
    }
    bool operator()(ICodeRunnerIdentifierPtr const& lhs, int id) const
    {
      return lhs->getID() < id;
    }

    bool operator()(const std::string str, ICodeRunnerIdentifierPtr const& rhs) const
    {
      return str < rhs->getName();
    }
    bool operator()(ICodeRunnerIdentifierPtr const& lhs, const std::string str) const
    {
      return lhs->getName() < str;
    }
  };


  class PlaygroundOrganizer final {
    struct PlaygroundOrganizerPimpl {
      using IPlaygroundObjMap = std::set<ICodeRunnerIdentifierPtr, CodeRunnerComp>;
      IPlaygroundObjMap m_pg_objects;
    };
  public:
    PlaygroundOrganizer() : pimpl_ptr(new PlaygroundOrganizerPimpl()) {
    }
    
    ~PlaygroundOrganizer() { }

    void Insert(std::initializer_list<ICodeRunnerIdentifierPtr> pg_objects)
    {
      // append given objects to the end of vector
      pimpl_ptr->m_pg_objects.insert(pg_objects);
    }

    void RunWithID(std::initializer_list<int> pg_ids)
    {
      for (auto id : pg_ids) {
        auto it = pimpl_ptr->m_pg_objects.find(id);
        if (it != pimpl_ptr->m_pg_objects.end())
        {
          (*it)->RunCode();
        }
      }
    }

    void RunAll()
    {
      std::for_each(
        pimpl_ptr->m_pg_objects.begin(),
        pimpl_ptr->m_pg_objects.end(),
        [](auto const& pg) { pg->RunCode(); });
    }

    std::string GetQuestionName(int id) {
      auto it = pimpl_ptr->m_pg_objects.find(id);
      return it != pimpl_ptr->m_pg_objects.end() ? (*it)->name : std::string();
    }

    int GetQuestionIDbyName(const std::string& name) {
      auto iter = pimpl_ptr->m_pg_objects.find(name);
      return iter != pimpl_ptr->m_pg_objects.end() ? (*iter)->id : -1; // or return directly first as id
    }

    void PrintDetails() {
      std::for_each(pimpl_ptr->m_pg_objects.begin(), pimpl_ptr->m_pg_objects.end(), [](auto const& q) {
        printf("\n%d. Playground id: %d\tname: %s", q->id, q->id, q->name.c_str());
        //std::cout
        //  << q->id << ". " << "Playground id: " << q->id << "\tname: " << q->name << std::endl;
      });
    }

  private:
    std::unique_ptr<PlaygroundOrganizerPimpl> pimpl_ptr;
  };

}

#endif _CPP_Playground_ORGANIZER_HPP_
