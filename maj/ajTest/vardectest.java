package maj.ajTest;


import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class vardectest {
	public static void main(String[] myargs)  {
		VarDec var1 = new VarDec(new Modifiers(2), new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))), new Identifier("x"));
		VarDec var2 = new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ArrayTypeD(new ResolvedTypeD(new MajUncheckedType(new Identifier("int")))))), new Identifier("x"));
		VarDec var3 = new VarDec(null, new ArrayTypeD(new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("int")))))), new Identifier("x"));
		VarDec var4 = new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("Foo"))))), new Identifier("x"));
		VarDec var5 = new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ArrayTypeD(new ResolvedTypeD(new MajUncheckedType(new Identifier("Foo")))))), new Identifier("x"));
		VarDec var6 = new VarDec(null, new ArrayTypeD(new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("Foo")))))), new Identifier("x"));
		VarDec var7 = new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier(new Identifier("Foo"), new Identifier("Bar")))))), new Identifier("x"));
		VarDec var8 = new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ArrayTypeD(new ResolvedTypeD(new MajUncheckedType(new Identifier(new Identifier("Foo"), new Identifier("Bar"))))))), new Identifier("x"));
		VarDec var9 = new VarDec(null, new ArrayTypeD(new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier(new Identifier("Foo"), new Identifier("Bar"))))))), new Identifier("x"));
		VarDec var10 = new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))), new Identifier("x"), new IntLiteralExpr(3));
		VarDec var11 = new VarDec(null, new ArrayTypeD(new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("int")))))), new Identifier("x"), new ArrayInitializer(new Exprs(new Expr[] {new IntLiteralExpr(1), new IntLiteralExpr(2), new IntLiteralExpr(3)})));
		VarDec var12 = new VarDec(null, new ArrayTypeD(new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("int")))))), new Identifier("x"), new ArrayInitializer(new Exprs(new Expr[] {new IntLiteralExpr(1), new IntLiteralExpr(2), new IntLiteralExpr(3)})));
		System.out.println(var1.unparse());
		System.out.println(var2.unparse());
		System.out.println(var3.unparse());
		System.out.println(var4.unparse());
		System.out.println(var5.unparse());
		System.out.println(var6.unparse());
		System.out.println(var7.unparse());
		System.out.println(var8.unparse());
		System.out.println(var9.unparse());
		System.out.println(var10.unparse());
		System.out.println(var11.unparse());
		System.out.println(var12.unparse());
		//		System.out.println(var13.unparse());
	}
}
