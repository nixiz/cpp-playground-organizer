#ifndef _CPP_Playground_ORGANIZER_HPP_
#define _CPP_Playground_ORGANIZER_HPP_

#define Stringize( L )     #L 
#define MakeString( M, L ) M(L)
#define $Line MakeString( Stringize, __LINE__ )
#define PrintError __FILE__ "(" $Line ") : Error: "
#define PrintWarn  __FILE__ "(" $Line ") : Warning: "

#include <memory>
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

#define ADD_ELEMENT(ClassName, ...) \
std::make_shared<ClassName>(__VA_ARGS__)

namespace CppOrganizer
{
  class PlaygroundOrganizer;

  class ICodeRunnerIdentifier
  {
    friend class PlaygroundOrganizer;
  public:
    explicit ICodeRunnerIdentifier(const std::string& _name);

    std::string getName() const;
    int getID() const;
  private:
    virtual void RunCode() {};
    const std::string name;
    const int id;
    static int quest_id;
  };
  typedef std::shared_ptr<ICodeRunnerIdentifier> ICodeRunnerIdentifierPtr;

  class CodeExecuteBenchmark {
  public:
    explicit CodeExecuteBenchmark(std::function<void()> f, const ICodeRunnerIdentifier& iden);
    ~CodeExecuteBenchmark();
  private:
    std::function<void()> f_;
    const ICodeRunnerIdentifier& iden_;
  };

  template <class T>
  class CodeRunnerHelper : public ICodeRunnerIdentifier
  {
  public: 
    CodeRunnerHelper(const std::string& testName = typeid(T).name()) : ICodeRunnerIdentifier(testName) { }
    virtual ~CodeRunnerHelper() {};

    void RunCode() override {
      CodeExecuteBenchmark {std::bind(&T::Run, static_cast<T *>(this)), *this};
    }

  protected:
#if COMPILE_WITH_UNIMPLEMENTED_RUNNER == 1
    void Run() {
      printf("\n%s\n%s::Run() should be implemented!\n", PrintWarn, typeid(T).name());;
    }
#endif
  };

  class PlaygroundOrganizer final
  {
    struct PlaygroundOrganizerPimpl;
  public:
    PlaygroundOrganizer();
    ~PlaygroundOrganizer();

    void Insert(std::initializer_list<ICodeRunnerIdentifierPtr> pg_objects);

    void RunWithID(std::initializer_list<int> pg_ids);

    void RunAll();

    std::string GetQuestionName(int id);

    int GetQuestionIDbyName(const std::string& name);

    void PrintDetails();

  private:
    std::unique_ptr<PlaygroundOrganizerPimpl> pimpl_ptr;
  };

}

#endif // _CPP_Playground_ORGANIZER_HPP_
