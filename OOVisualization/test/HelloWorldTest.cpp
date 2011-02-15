/***********************************************************************************************************************
 * HelloWorldTest.cpp
 *
 *  Created on: Feb 1, 2011
 *      Author: Dimitar Asenov
 **********************************************************************************************************************/

#include "oovisualization.h"
#include "SelfTest/headers/SelfTestSuite.h"
#include "vis/VVisibility.h"
#include "vis/VStatic.h"

#include "OOModel/headers/allOOModelNodes.h"

#include "VisualizationBase/headers/VisualizationManager.h"
#include "VisualizationBase/headers/Scene.h"
#include "VisualizationBase/headers/views/MainView.h"
#include "VisualizationBase/headers/ModelRenderer.h"
#include "VisualizationBase/headers/items/VExtendable.h"
#include "VisualizationBase/headers/items/VText.h"
#include "VisualizationBase/headers/items/VList.h"
#include "VisualizationBase/headers/node_extensions/Position.h"

#include "ModelBase/headers/Model.h"

#include <QtGui/QDesktopWidget>
#include <QtGui/QApplication>
#include <QtCore/QCoreApplication>

using namespace OOModel;
using namespace Visualization;

namespace OOVisualization {

TEST(OOVisualization, JavaLibraryAndHelloWorldTest)
{
	Scene* scene = new Scene();

	scene->defaultRenderer()->registerVisualization(Visibility::typeIdStatic(), createVisualization<VVisibility, Visibility>);
	scene->defaultRenderer()->registerVisualization(Static::typeIdStatic(), createVisualization<VStatic, Static>);

	////////////////////////////////////////////////// Create Model
	Model::Model* model = new Model::Model();
	Project* prj = dynamic_cast<Project*> (model->createRoot("Project"));

	model->beginModification(prj, "build simple java library and a hello world app");
	prj->setName("HelloWorld");

	// Build a simple Java Library
	Library* java = prj->libraries()->append<Library>();
	java->setName("Java");

	Class* string = java->classes()->append<Class>();
	string->setName("String");
	string->setVisibility(Visibility::PUBLIC);

	Module* io = java->modules()->append<Module>();
	io->setName("io");

	Class* printstream = io->classes()->append<Class>();
	printstream->setName("PrintStream");
	printstream->setVisibility(Visibility::PUBLIC);

	Method* println = printstream->methods()->append<Method>();
	println->setName("println");
	println->setVisibility(Visibility::PUBLIC);

	FormalArgument* arg = println->arguments()->append<FormalArgument>();
	arg->setName("x");
	NamedType* argType = arg->setType<NamedType>();
	argType->type()->ref()->set("class:String");

	Class* system = java->classes()->append<Class>();
	system->setName("System");
	system->setVisibility(Visibility::PUBLIC);
	Field* out = system->fields()->append<Field>();
	out->setName("out");
	out->setVisibility(Visibility::PUBLIC);
	out->setStat(Static::CLASS_VARIABLE);
	NamedType* outtype = out->setType<NamedType>();
	outtype->type()->ref()->set("class:PrintStream");
	outtype->type()->setPrefix<ReferenceExpression>()->ref()->set("mod:io");

	// Build a simple HelloWorld Application
	Class* hello = prj->classes()->append<Class>();
	hello->setName("HelloWorld");
	hello->setVisibility(Visibility::PUBLIC);
	Method* main = hello->methods()->append<Method>();

	main->setName("main");
	main->setVisibility(Visibility::PUBLIC);
	main->setStat(Static::CLASS_VARIABLE);
	//TODO make an array argument

	MethodCallStatement* callPrintln = main->items()->append<MethodCallStatement>();
	StringLiteral* helloStr = callPrintln->arguments()->append<StringLiteral>();
	helloStr->setValue("Hello World");
	callPrintln->ref()->set("met:println");

	VariableAccess* va = callPrintln->setPrefix<VariableAccess>();
	va->ref()->set("field:out");

	ReferenceExpression* ref = va->setPrefix<ReferenceExpression>();
	ref->ref()->set("class:System");
	ref->setPrefix<ReferenceExpression>()->ref()->set("lib:Java");

	// Add a second method
	Method* factorial = hello->methods()->append<Method>();
	factorial->setName("factorial");
	factorial->results()->append<FormalResult>()->setType<PrimitiveType>()->setType(PrimitiveType::INT);
	factorial->arguments()->append<FormalArgument>()->setType<PrimitiveType>()->setType(PrimitiveType::INT);
	factorial->arguments()->at(0)->setName("x");

	VariableDeclaration* var1 = factorial->items()->append<VariableDeclaration>();
	var1->setName("var1");
	var1->setType<PrimitiveType>()->setType(PrimitiveType::INT);

	VariableDeclaration* var2 = factorial->items()->append<VariableDeclaration>();
	var2->setName("var2");
	var2->setType<PrimitiveType>()->setType(PrimitiveType::LONG);
	var2->setInitialValue<IntegerLiteral>()->setValue(42);

	VariableDeclaration* var3 = factorial->items()->append<VariableDeclaration>();
	var3->setName("var3");
	var3->setType<PrimitiveType>()->setType(PrimitiveType::BOOLEAN);
	var3->setInitialValue<BooleanLiteral>()->setValue(true);

	VariableDeclaration* var4 = factorial->items()->append<VariableDeclaration>();
	var4->setName("var4");
	var4->setType<PrimitiveType>()->setType(PrimitiveType::CHAR);
	var4->setInitialValue<CharacterLiteral>()->setValue('r');

	VariableDeclaration* var5 = factorial->items()->append<VariableDeclaration>();
	var5->setName("var5");
	var5->setType<PrimitiveType>()->setType(PrimitiveType::DOUBLE);
	var5->setInitialValue<FloatLiteral>()->setValue(123.112311096123);

	VariableDeclaration* var6 = factorial->items()->append<VariableDeclaration>();
	var6->setName("var6");
	var6->setType<PrimitiveType>()->setType(PrimitiveType::UNSIGNED_LONG);
	var6->setInitialValue<IntegerLiteral>()->setValue(1000);

	VariableDeclaration* var7 = factorial->items()->append<VariableDeclaration>();
	var7->setName("var7");
	var7->setType<PrimitiveType>()->setType(PrimitiveType::VOID);
	var7->setInitialValue<NullLiteral>();

	VariableDeclaration* var8 = factorial->items()->append<VariableDeclaration>();
	var8->setName("var8");
	var8->setType<PrimitiveType>()->setType(PrimitiveType::INT);
	var8->setInitialValue<MethodCallExpression>()->ref()->set("met:getId");

	VariableDeclaration* var9 = factorial->items()->append<VariableDeclaration>();
	var9->setName("var9");
	var9->setType<PrimitiveType>()->setType(PrimitiveType::VOID);
	var9->setInitialValue<ThisExpression>();

	VariableDeclaration* var10 = factorial->items()->append<VariableDeclaration>();
	var10->setName("var10");
	var10->setType<PrimitiveType>()->setType(PrimitiveType::INT);
	CastExpression* cast = var10->setInitialValue<CastExpression>();
	cast->setType<PrimitiveType>()->setType(PrimitiveType::INT);
	cast->setExpr<StringLiteral>()->setValue("five");

	VariableDeclaration* var11 = factorial->items()->append<VariableDeclaration>();
	var11->setName("var11");
	var11->setType<PrimitiveType>()->setType(PrimitiveType::VOID);
	var11->setInitialValue<NewExpression>()->setType<PrimitiveType>()->setType(PrimitiveType::INT);

	VariableDeclaration* var12 = factorial->items()->append<VariableDeclaration>();
	var12->setName("var12");
	var12->setType<PrimitiveType>()->setType(PrimitiveType::VOID);
	NewExpression* newExpr = var12->setInitialValue<NewExpression>();
	newExpr->setType<PrimitiveType>()->setType(PrimitiveType::INT);
	newExpr->setAmount<VariableAccess>()->ref()->set("local:var2");

	VariableDeclaration* var13 = factorial->items()->append<VariableDeclaration>();
	var13->setName("var13");
	var13->setType<PrimitiveType>()->setType(PrimitiveType::BOOLEAN);
	UnaryOperation* uOp1 = var13->setInitialValue<UnaryOperation>();
	uOp1->setOp(UnaryOperation::NOT);
	uOp1->setOperand<BooleanLiteral>()->setValue(false);

	VariableDeclaration* var14 = factorial->items()->append<VariableDeclaration>();
	var14->setName("var14");
	var14->setType<PrimitiveType>()->setType(PrimitiveType::INT);
	UnaryOperation* uOp2 = var14->setInitialValue<UnaryOperation>();
	uOp2->setOp(UnaryOperation::INCREMENT);
	uOp2->setOperand<IntegerLiteral>()->setValue(10);

	VariableDeclaration* var15 = factorial->items()->append<VariableDeclaration>();
	var15->setName("var15");
	var15->setType<PrimitiveType>()->setType(PrimitiveType::INT);
	BinaryOperation* binOp1 = var15->setInitialValue<BinaryOperation>();
	binOp1->setOp(BinaryOperation::PLUS);
	binOp1->setLeft<IntegerLiteral>()->setValue(41);
	binOp1->setRight<IntegerLiteral>()->setValue(1);

	VariableDeclaration* var16 = factorial->items()->append<VariableDeclaration>();
	var16->setName("var16");
	var16->setType<PrimitiveType>()->setType(PrimitiveType::BOOLEAN);
	BinaryOperation* binOp2 = var16->setInitialValue<BinaryOperation>();
	binOp2->setOp(BinaryOperation::LESS_EQUALS);
	binOp2->setLeft<IntegerLiteral>()->setValue(41);
	binOp2->setRight<IntegerLiteral>()->setValue(1);

	Block* block = factorial->items()->append<Block>();

	AssignmentStatement* assign = block->items()->append<AssignmentStatement>();
	assign->setLeft<VariableAccess>()->ref()->set("local:var1");
	assign->setRight<IntegerLiteral>()->setValue(84);

	factorial->items()->append<ReturnStatement>()->values()->append<IntegerLiteral>()->setValue(24);

	// set positions
	factorial->extension<Position>()->setY(100);
	java->extension<Position>()->setX(160);
	java->extension<Position>()->setY(100);
	string->extension<Position>()->setY(100);
	io->extension<Position>()->setX(240);

	model->endModification();
	CHECK_INT_EQUAL(160, java->extension<Position>()->x());
	CHECK_STR_EQUAL("Java", java->name());

	////////////////////////////////////////////////// Set Scene

	scene->addTopLevelItem( scene->defaultRenderer()->render(NULL, prj) );
	scene->scheduleUpdate();

	// Create view
	MainView* view = new MainView(scene);

	CHECK_CONDITION(view != NULL);
}

}
