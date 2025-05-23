package maj.ajTest;


import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class methodtest {
	public static void main(String[] myargs)  {
		ASTObject mods1 = new Modifiers(9);
		ASTObject meth1 = new MethodDec(null, new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))), "foo", new Formals(new FormalDec[] {new FormalDec(null, new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))), "x"), new FormalDec(null, new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))), "y")}), new TypeDs(new ResolvedTypeD[] {new ResolvedTypeD(new MajUncheckedType(new Identifier("oneexception"))), new ResolvedTypeD(new MajUncheckedType(new Identifier("anotherexcepion")))}), new CodeBody(new Stmts(new Stmt[] {new VarDec(mods1, new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))), new Identifier("z")), new WhileStmt(new NumericTestOpExpr(new VarExpr(new Identifier("z")), ">", new IntLiteralExpr(0)), new CodeBody(new Stmts(new Stmt[] {new ExprStmt(new PostfixExpr(new VarExpr(new Identifier("x")), "++")), new IfStmt(new NumericTestOpExpr(new VarExpr(new Identifier("x")), ">", new IntLiteralExpr(5)), new BreakStmt(null), null)}))), new ReturnStmt(new VarExpr(new Identifier("x")))})));
		ASTObject cons1 = new MethodDec(null, null, "foo", new Formals(new FormalDec[] {new FormalDec(null, new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))), "x"), new FormalDec(null, new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))), "y")}), new TypeDs(new ResolvedTypeD[] {new ResolvedTypeD(new MajUncheckedType(new Identifier("something")))}), new CodeBody(new Stmts(new Stmt[] {new ExprStmt(new PostfixExpr(new VarExpr(new Identifier("x")), "++")), new ReturnStmt(new VarExpr(new Identifier("x")))})));
		ASTObject meth1A = new MethodDec(null, new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))), "foo", new Formals(new FormalDec[] {new FormalDec(null, new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))), "x"), new FormalDec(null, new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))), "y")}), null, new CodeBody(new Stmts(new Stmt[] {new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))), new Identifier("z")), new ExprStmt(new PostfixExpr(new VarExpr(new Identifier("x")), "++")), new ReturnStmt(new VarExpr(new Identifier("x")))})));
		ASTObject var1 = new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))), new Identifier("x"));
		ASTObject meth1B = new MethodDec(null, new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))), "foo", new Formals(new FormalDec[] {new FormalDec(null, new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))), "x"), new FormalDec(null, new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))), "y")}), null, new CodeBody(new Stmts(new Stmt[] {var1, new ExprStmt(new PostfixExpr(new VarExpr(new Identifier("x")), "++")), new ReturnStmt(new VarExpr(new Identifier("x")))})));
		ASTObject stat1 = new ExprStmt(new PostfixExpr(new VarExpr(new Identifier("x")), "++"));
		ASTObject meth1D = new MethodDec(null, new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))), "foo", new Formals(new FormalDec[] {new FormalDec(null, new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))), "x"), new FormalDec(null, new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))), "y")}), null, new CodeBody(new Stmts(new Stmt[] {new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))), new Identifier("z")), stat1, new ReturnStmt(new VarExpr(new Identifier("x")))})));
		ASTObject meth2 = new MethodDec(null, new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier(new Identifier("Foo"), new Identifier("Bar")))))), "baz", new Formals(new FormalDec[] {new FormalDec(null, new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))), "x"), new FormalDec(null, new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))), "y")}), null, null);
		System.out.println(meth1.unparse());
		System.out.println(cons1.unparse());
		System.out.println(meth1A.unparse());
		System.out.println(meth1B.unparse());
		System.out.println(meth1D.unparse());
		System.out.println(meth2.unparse());
	}
}
