#include "utils.h"
#include "parser/parser.h"
#include "parser/symbol-registry.h"
#include "ast/ast.h"
#include "ast/node-factory.h"


using namespace Swift;

class TestPostfixExpression : public SwiftTestCase
{
    CPPUNIT_TEST_SUITE(TestPostfixExpression);
    CPPUNIT_TEST(testPostfixOperator);
    CPPUNIT_TEST(testFunctionCall);
    CPPUNIT_TEST(testInit);
    CPPUNIT_TEST(testSelf);
    CPPUNIT_TEST(testDynamicType);
    CPPUNIT_TEST(testSubscript);
    CPPUNIT_TEST(testSubscript2);
    CPPUNIT_TEST(testSubscript3);
    CPPUNIT_TEST(testForcedValue);
    CPPUNIT_TEST(testOptionalChaining);
    CPPUNIT_TEST_SUITE_END();
public:
    
    void testPostfixOperator()
    {
        PARSE_STATEMENT(L"i++ % 3");
        CPPUNIT_ASSERT(root != NULL);
        BinaryOperator* mod = dynamic_cast<BinaryOperator*>(root);
        CPPUNIT_ASSERT(mod != NULL);
        ASSERT_EQUALS(L"%", mod->getOperator().c_str());
        CPPUNIT_ASSERT(mod->getLHS() != NULL);
        UnaryOperator* inc = dynamic_cast<UnaryOperator*>(mod->getLHS());
        CPPUNIT_ASSERT(inc != NULL);
        ASSERT_EQUALS(L"++", inc->getOperator().c_str());
        CPPUNIT_ASSERT_EQUAL(OperatorType::PostfixUnary, inc->getType());
        
        DESTROY(root);
    }
    
    
    void testFunctionCall()
    {
        PARSE_STATEMENT(L"foo.bar(1, 2 + 3, id : 5)");
        CPPUNIT_ASSERT(root != NULL);
        FunctionCall* call = dynamic_cast<FunctionCall*>(root);
        CPPUNIT_ASSERT(call != NULL);
        CPPUNIT_ASSERT(call->getFunction() != NULL);
        MemberAccess* member = dynamic_cast<MemberAccess*>(call->getFunction());
        CPPUNIT_ASSERT(member != NULL);

        Identifier* id = dynamic_cast<Identifier*>(member->getSelf());
        CPPUNIT_ASSERT(id != NULL);
        ASSERT_EQUALS(L"foo", id->getIdentifier().c_str());
        id = dynamic_cast<Identifier*>(member->getField());
        CPPUNIT_ASSERT(id != NULL);
        ASSERT_EQUALS(L"bar", id->getIdentifier().c_str());
        
        ParenthesizedExpression* args = call->getArguments();
        CPPUNIT_ASSERT(args != NULL);
        CPPUNIT_ASSERT_EQUAL(3, args->numExpressions());
        
        Node* n = args->get(0);
        CPPUNIT_ASSERT(n != NULL);
        IntegerLiteral* i = dynamic_cast<IntegerLiteral*>(n);
        CPPUNIT_ASSERT(i != NULL);
        ASSERT_EQUALS(L"1", i->toString().c_str());
        
        n = args->get(1);
        CPPUNIT_ASSERT(n != NULL);
        BinaryOperator* op = dynamic_cast<BinaryOperator*>(n);
        CPPUNIT_ASSERT(op != NULL);
        ASSERT_EQUALS(L"+", op->getOperator().c_str());
        
        CPPUNIT_ASSERT(i = dynamic_cast<IntegerLiteral*>(op->getLHS()));
        ASSERT_EQUALS(L"2", i->toString().c_str());
        
        
        CPPUNIT_ASSERT(i = dynamic_cast<IntegerLiteral*>(op->getRHS()));
        ASSERT_EQUALS(L"3", i->toString().c_str());
        
        

        std::wstring name = args->getName(2);
        ASSERT_EQUALS(L"id", name.c_str());
        
        n = args->get(2);
        CPPUNIT_ASSERT(n != NULL);
        CPPUNIT_ASSERT(i = dynamic_cast<IntegerLiteral*>(n));
        ASSERT_EQUALS(L"5", i->toString().c_str());
        
        
        DESTROY(root);
        
    }
    
    void testInit()
    {
        
        PARSE_STATEMENT(L"Shape.init(id : 5)");
        CPPUNIT_ASSERT(root != NULL);
        FunctionCall* call = dynamic_cast<FunctionCall*>(root);
        CPPUNIT_ASSERT(call != NULL);
        CPPUNIT_ASSERT(call->getFunction() != NULL);
        InitializerReference* init = dynamic_cast<InitializerReference*>(call->getFunction());
        CPPUNIT_ASSERT(init != NULL);

        Identifier* id = dynamic_cast<Identifier*>(init->getExpression());
        CPPUNIT_ASSERT(id != NULL);
        ASSERT_EQUALS(L"Shape", id->getIdentifier().c_str());
        
        ParenthesizedExpression* args = call->getArguments();
        CPPUNIT_ASSERT(args != NULL);
        CPPUNIT_ASSERT_EQUAL(1, args->numExpressions());
        
        Node* n = args->get(0);
        CPPUNIT_ASSERT(n != NULL);
        IntegerLiteral* i = dynamic_cast<IntegerLiteral*>(n);
        CPPUNIT_ASSERT(i != NULL);
        ASSERT_EQUALS(L"5", i->toString().c_str());
        
        std::wstring name = args->getName(0);
        ASSERT_EQUALS(L"id", name.c_str());
        
        DESTROY(root);
    }
    
    void testSelf()
    {
        PARSE_STATEMENT(L"Shape.self");
        CPPUNIT_ASSERT(root != NULL);
        SelfExpression* self = dynamic_cast<SelfExpression*>(root);
        CPPUNIT_ASSERT(self != NULL);
        CPPUNIT_ASSERT(self->getExpression());
        Identifier* id = dynamic_cast<Identifier*>(self->getExpression());
        CPPUNIT_ASSERT(id != NULL);
        ASSERT_EQUALS(L"Shape", id->getIdentifier().c_str());
        
        DESTROY(root);
    }
    
    void testDynamicType()
    {
        PARSE_STATEMENT(L"345.dynamicType");
        CPPUNIT_ASSERT(root != NULL);
        DynamicType* dyn = dynamic_cast<DynamicType*>(root);
        CPPUNIT_ASSERT(dyn != NULL);
        CPPUNIT_ASSERT(dyn->getExpression());
        IntegerLiteral* id = dynamic_cast<IntegerLiteral*>(dyn->getExpression());
        CPPUNIT_ASSERT(id != NULL);
        ASSERT_EQUALS(L"345", id->toString().c_str());
        
        DESTROY(root);
    }
    
    
    void testSubscript()
    {
        {
            PARSE_STATEMENT(L"matrix[0, 1] = 2");
            CPPUNIT_ASSERT(root != NULL);
            Assignment* eq = dynamic_cast<Assignment*>(root);
            CPPUNIT_ASSERT(eq != NULL);
            CPPUNIT_ASSERT(eq->getLHS());
            SubscriptAccess* sub = dynamic_cast<SubscriptAccess*>(eq->getLHS());
            CPPUNIT_ASSERT(eq != NULL);
            CPPUNIT_ASSERT_EQUAL(2, sub->numIndices());
            IntegerLiteral* i;
            
            CPPUNIT_ASSERT(i = dynamic_cast<IntegerLiteral*>(sub->getIndex(0)));
            ASSERT_EQUALS(L"0", i->toString().c_str());
            CPPUNIT_ASSERT(i = dynamic_cast<IntegerLiteral*>(sub->getIndex(1)));
            ASSERT_EQUALS(L"1", i->toString().c_str());
            
            DESTROY(root);
        }
        
    }
    void testSubscript2()
    {
        PARSE_STATEMENT(L"matrix[0, ] = 2");
        CPPUNIT_ASSERT(root != NULL);
        Assignment* eq = dynamic_cast<Assignment*>(root);
        CPPUNIT_ASSERT(eq != NULL);
        CPPUNIT_ASSERT(eq->getLHS());
        SubscriptAccess* sub = dynamic_cast<SubscriptAccess*>(eq->getLHS());
        CPPUNIT_ASSERT(eq != NULL);
        CPPUNIT_ASSERT_EQUAL(1, sub->numIndices());
        IntegerLiteral* i;
        
        CPPUNIT_ASSERT(i = dynamic_cast<IntegerLiteral*>(sub->getIndex(0)));
        ASSERT_EQUALS(L"0", i->toString().c_str());
        
        DESTROY(root);
    }
    void testSubscript3()
    {
        PARSE_STATEMENT(L"matrix[0 ] = 2");
        CPPUNIT_ASSERT(root != NULL);
        Assignment* eq = dynamic_cast<Assignment*>(root);
        CPPUNIT_ASSERT(eq != NULL);
        CPPUNIT_ASSERT(eq->getLHS());
        SubscriptAccess* sub = dynamic_cast<SubscriptAccess*>(eq->getLHS());
        CPPUNIT_ASSERT(eq != NULL);
        CPPUNIT_ASSERT_EQUAL(1, sub->numIndices());
        IntegerLiteral* i;
        
        CPPUNIT_ASSERT(i = dynamic_cast<IntegerLiteral*>(sub->getIndex(0)));
        ASSERT_EQUALS(L"0", i->toString().c_str());
        
        DESTROY(root);
        
    }
    void testForcedValue()
    {
        PARSE_STATEMENT(L"val!");
        CPPUNIT_ASSERT(root != NULL);
        ForcedValue* val = dynamic_cast<ForcedValue*>(root);
        CPPUNIT_ASSERT(val != NULL);
        CPPUNIT_ASSERT(val->getExpression());
        Identifier* id = dynamic_cast<Identifier*>(val->getExpression());
        CPPUNIT_ASSERT(id != NULL);
        ASSERT_EQUALS(L"val", id->getIdentifier().c_str());
        
        DESTROY(root);
    }
    
    void testOptionalChaining()
    {
        PARSE_STATEMENT(L"c?.performAction()");
        CPPUNIT_ASSERT(root != NULL);
        FunctionCall* call = dynamic_cast<FunctionCall*>(root);
        CPPUNIT_ASSERT(call != NULL);
        CPPUNIT_ASSERT(call->getFunction());
        MemberAccess* mem = dynamic_cast<MemberAccess*>(call->getFunction());
        CPPUNIT_ASSERT(mem != NULL);
        OptionalChaining* optional = dynamic_cast<OptionalChaining*>(mem->getSelf());
        CPPUNIT_ASSERT(optional != NULL);
        Identifier* id = dynamic_cast<Identifier*>(optional->getExpression());
        CPPUNIT_ASSERT(id != NULL);
        ASSERT_EQUALS(L"c", id->getIdentifier().c_str());
        
        DESTROY(root);
        
    }
    
};
;

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestPostfixExpression, "alltest");
