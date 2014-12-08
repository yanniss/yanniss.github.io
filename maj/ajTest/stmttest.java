package maj.ajTest;


import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class stmttest {
	public static void main(String[] myargs)  {
		VarDec var1 = new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))), new Identifier("x"));
		Expr expr1 = new VarExpr(new Identifier("foo"));
		Stmt stat1 = new ExprStmt(new PostfixExpr(new VarExpr(new Identifier("x")), "++"));
		Stmt stat1A = new ExprStmt(new NumericOpExpr(new VarExpr(new Identifier("x")), "=", new IntLiteralExpr(0)));
		Stmt stat2 = new DoStmt(new CodeBody(new Stmts(new Stmt[] {})), new BooleanLiteralExpr(true));
		Stmt stat3 = new ForStmt(new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))), new Identifier("x"), new IntLiteralExpr(0)), new NumericTestOpExpr(new VarExpr(new Identifier("x")), "!=", new IntLiteralExpr(5)), new Exprs(new Expr[] {new PostfixExpr(new VarExpr(new Identifier("x")), "++")}), new CodeBody(new Stmts(new Stmt[] {new ExprStmt(new NumericOpExpr(new VarExpr(new Identifier("y")), "=", new CastExpr(new ResolvedTypeD(new MajUncheckedType(new Identifier("Foo"))), new CallExpr(null, "Bar", new Exprs(new Expr[] {new VarExpr(new Identifier("x"))})))))})));
		Stmt stat3A = new ForStmt(var1, new NumericTestOpExpr(new VarExpr(new Identifier("x")), "!=", new IntLiteralExpr(5)), new Exprs(new Expr[] {new PostfixExpr(new VarExpr(new Identifier("x")), "++")}), new CodeBody(new Stmts(new Stmt[] {new ExprStmt(new CallExpr(null, "Bar", new Exprs(new Expr[] {new VarExpr(new Identifier("x"))})))})));
		Stmt stat4 = new IfStmt(new NumericTestOpExpr(new VarExpr(new Identifier("x")), "==", new VarExpr(new Identifier("y"))), new ExprStmt(new PostfixExpr(new VarExpr(new Identifier("x")), "++")), new ExprStmt(new PostfixExpr(new VarExpr(new Identifier("y")), "++")));
		Stmt stat4A = new IfStmt(expr1, new ExprStmt(new PostfixExpr(new VarExpr(new Identifier("x")), "++")), new ExprStmt(new PostfixExpr(new VarExpr(new Identifier("y")), "++")));
		Stmt stat4A1 = new IfStmt(expr1, new ExprStmt(new PostfixExpr(new VarExpr(new Identifier("x")), "++")), new ExprStmt(new PostfixExpr(new VarExpr(new Identifier("y")), "++")));
		Stmt stat4B = new IfStmt(new NumericTestOpExpr(new VarExpr(new Identifier("x")), "==", new VarExpr(new Identifier("y"))), stat1, new ExprStmt(new PostfixExpr(new VarExpr(new Identifier("y")), "++")));
		Stmt stat4C = new IfStmt(new NumericTestOpExpr(new VarExpr(new Identifier("x")), "==", new VarExpr(new Identifier("y"))), new ExprStmt(new PostfixExpr(new VarExpr(new Identifier("x")), "++")), stat2);
		Stmt stat4D = new IfStmt(new NumericTestOpExpr(new VarExpr(new Identifier("x")), "==", new VarExpr(new Identifier("y"))), new ExprStmt(new PostfixExpr(new VarExpr(new Identifier("x")), "++")), new CodeBody(new Stmts(new Stmt[] {var1})));
		Stmt statE = new IfStmt(new NumericTestOpExpr(new VarExpr(new Identifier("x")), "==", new VarExpr(new Identifier("y"))), new ExprStmt(new PostfixExpr(new VarExpr(new Identifier("x")), "++")), new CodeBody(new Stmts(new Stmt[] {})));
		Stmt selfstat2 = stat1;
		System.out.println(stat1.unparse());
		System.out.println(stat1A.unparse());
		System.out.println(stat2.unparse());
		System.out.println(stat3.unparse());
		System.out.println(stat3A.unparse());
		System.out.println(stat4.unparse());
		System.out.println(stat4A.unparse());
		System.out.println(stat4A1.unparse());
		System.out.println(stat4B.unparse());
		System.out.println(stat4C.unparse());
		System.out.println(stat4D.unparse());
		System.out.println(statE.unparse());
		System.out.println(selfstat2.unparse());
	}
}
