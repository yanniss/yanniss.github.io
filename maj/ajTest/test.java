package maj.ajTest;

import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class test {

    public static void main(String myargs[]) {
	// Identifier
	infer id1 = `[foo];
	Identifier id2 = `[foo];
	infer id3 = `(Identifier)[foo];
	Identifier id4 = `(Identifier)[foo];
	infer pack1 = `[foo.bar];
	infer pack2 = `[java.lang.#pack1];
	infer selfid1 = `(Identifier)[#pack1];
	infer selfid2 = `(Identifier)[#(Identifier)pack1];

	System.out.println(id1.unparse());
	System.out.println(id2.unparse());
	System.out.println(id3.unparse());
	System.out.println(id4.unparse());
	System.out.println(pack1.unparse());
	System.out.println(pack2.unparse());
	
	// Name Pattern
	infer name1 = `[*];
	NamePattern name2 = `[Foo.*];
	infer name3 = `(NamePattern)[Foo.*];
	NamePattern name4 = `(NamePattern)[foo];
	infer packStar = `[java.lang.*];

	infer selfname1 = `(NamePattern)[#pack1];
	infer selfname2 = `(NamePattern)[#(NamePattern)pack1];

	System.out.println(name1.unparse());
	System.out.println(name2.unparse());
	System.out.println(name3.unparse());
	System.out.println(packStar.unparse());

	// Modifiers
	infer mods1 = `[public static];
	Modifiers mods2 = `[public static];
	infer mods3 = `(Modifiers)[public static];
	Modifiers mods4 = `(Modifiers)[public static];
	infer selfmods1 = `(Modifiers)[#mods4];
	infer selfmods2 = `(Modifiers)[#(Modifiers)mods4];	
	//???	infer odd = `[];

	System.out.println(mods1.unparse());
	System.out.println(mods2.unparse());
	System.out.println(mods3.unparse());
	System.out.println(mods4.unparse());
	System.out.println(selfmods1.unparse());
	System.out.println(selfmods2.unparse());

	// Import
	infer imp1 = `[import java.lang.*;];
	Import imp2 = `[import java.lang.*;];
	infer imp3 = `(Import)[import java.lang.*;];
	Import imp4 = `(Import)[import java.lang.*;];
	infer selfimp1 = `(Import)[#imp4];
	infer selfimp2 = `(Import)[#(Import)imp4];

	System.out.println(imp1.unparse());
	System.out.println(imp2.unparse());
	System.out.println(imp3.unparse());
	System.out.println(imp3.unparse());
	System.out.println(selfimp1.unparse());
	System.out.println(selfimp2.unparse());

	// Type Spec
	infer type1 = `[int];
	infer type2 = `[int[]];
	infer type3 = `[int[][]];
	TypeD type4 = `[int];
	infer type5 = `(TypeD)[int];
	TypeD type6 = `(TypeD)[int[]];
	TypeD type7 = `(TypeD)[Foo.Bar];
	TypeD type8 = `(TypeD)[#type1];
	TypeD type9 = `(TypeD)[#(Identifier)type1];
	TypeD type10 = `(TypeD)[#type1[]];
	infer selftyp1 = `(TypeD)[#type4];
	infer selftyp2 = `(TypeD)[#(TypeD)type4];

	System.out.println(type1.unparse());
	System.out.println(type2.unparse());
	System.out.println(type3.unparse());
	System.out.println(type4.unparse());
	System.out.println(type5.unparse());
	System.out.println(type6.unparse());
	System.out.println(type7.unparse());
	System.out.println(type8.unparse());
	System.out.println(type9.unparse());
	System.out.println(selftyp1.unparse());
	System.out.println(selftyp2.unparse());
	
	// Expression
	infer expr1 = `(Expr)[foo];
	infer expr2 = `(Expr)[32];
	infer expr3 = `(Expr)["abc"];
	infer expr4 = `(Expr)[32.30F];
	infer expr5 = `(Expr)[true];
	infer expr6 = `(Expr)[false];
	infer expr7 = `(Expr)[null];
	infer expr8 = `(Expr)[this];
	infer expr9 = `(Expr)[super];
	infer expr10 = `(Expr)[foo--];
	infer expr11 = `(Expr)[a.super(a, b, c)];
	infer expr12 = `(Expr)[~foo];
	infer expr13 = `(Expr)[!foo];
	infer expr14 = `(Expr)[-foo];
	infer expr15 = `(Expr)[+foo];
	infer expr16 = `(Expr)[++foo];
	infer expr17 = `(Expr)[--foo];
	infer expr18 = `(Expr)[1*3/2*2];
	infer expr19 = `(Expr)[1*3 - 4/2];
	infer expr20 = `(Expr)[1 >> foo];
	infer expr21 = `(Expr)[1 > 1];
	/*
	infer cond1 =  `[x==y];
	infer cond2 =  `(Expr)[x==y];
	infer selfexp1 = `(Expr)[#cond1];
	infer selfexp2 = `(Expr)[#(Expr)cond1];
	*/

	System.out.println(expr1.unparse());
	System.out.println(expr2.unparse());
	System.out.println(expr3.unparse());
	System.out.println(expr4.unparse());
	System.out.println(expr5.unparse());
	System.out.println(expr6.unparse());
	System.out.println(expr7.unparse());
	System.out.println(expr8.unparse());
	System.out.println(expr9.unparse());
	System.out.println(expr10.unparse());
	System.out.println(expr11.unparse());
	System.out.println(expr12.unparse());
	System.out.println(expr13.unparse());
	System.out.println(expr14.unparse());
	System.out.println(expr15.unparse());
	System.out.println(expr16.unparse());
	System.out.println(expr17.unparse());
	System.out.println(expr18.unparse());
	System.out.println(expr19.unparse());
	System.out.println(expr20.unparse());

	// Var. Dec.
	/*	infer var1 =  `[int x;];
	infer var2 =  `(VarDec)[int x;];
	infer var3 =  `[int #id1;];*/	//VariableDeclarator uses IDENT (should we allow Identifier in there?
/*
	infer var4 =  `[int #(Identifier)id1;];
	infer var5 =  `[#type1 #id1;];
	infer selfvar1 = `(VarDec)[#var1];
	infer selfvar2 = `(VarDec)[#(VarDec)var1];

	// Statement
	infer stata =  `[if ( #(Expr)cond2 ) x++; else y++; ];
	infer statb =  `[x++;];

	infer stat1 =  `(Statement)[if ( #(Expr)cond2 ) x++; else y++;];
	infer stat2 =  `(Statement)[if ( #cond2 ) x++; else y++;];
	infer stat3 =  `(Statement)[{#(VarDec)var1}];
	infer stat3 =  `(Statement)[{#(ClassDec)class11}];
	infer selfstat1 = `(Statement)[#stat1];
	infer selfstat2 = `(Statement)[#(Statement)stat1];

	// Class Declaration
	infer class1 =  `[ class foo { } ];	//Inference(quote)
	infer class2 =  `(ClassDec)[ class foo { int x; #var1 } ];	// Ambiguous
*/
	/*
	try {
	    File outputFile = new File("/home/shuang/research/maj/ajTest/test_trans.java");
	    FileWriter out = new FileWriter(outputFile);
	    
	    out.write(((maj.ExtendedAST)myAST).toSource());
	    out.close();
	} catch (Exception e) {
	    e.printStackTrace();
	    }*/
    }
}
