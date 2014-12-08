package maj.ajTest;


import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class exprtest {
	public static void main(String[] myargs)  {
		int[] var = {1, 2, 3};
		ASTObject expr1 = new VarExpr(new Identifier("foo"));
		ASTObject expr2 = new IntLiteralExpr(32);
		ASTObject expr3 = new StringLiteralExpr("\"abc\"");
		ASTObject expr4 = new FloatLiteralExpr(32.30F);
		ASTObject expr5 = new BooleanLiteralExpr(true);
		ASTObject expr6 = new BooleanLiteralExpr(false);
		ASTObject expr7 = new NullExpr();
		ASTObject expr8 = new ThisExpr();
		ASTObject expr9 = new SuperExpr();
		ASTObject expr10 = new PostfixExpr(new VarExpr(new Identifier("foo")), "--");
		ASTObject expr11 = new CallExpr(new VarExpr(new Identifier("a")), "super", new Exprs(new Expr[] {new VarExpr(new Identifier("a")), new VarExpr(new Identifier("b")), new VarExpr(new Identifier("c"))}));
		ASTObject expr12 = UnopExpr.build("~", new VarExpr(new Identifier("foo")));
		ASTObject expr13 = UnopExpr.build("!", new VarExpr(new Identifier("foo")));
		ASTObject expr14 = UnopExpr.build("-", new VarExpr(new Identifier("foo")));
		ASTObject expr15 = UnopExpr.build("+", new VarExpr(new Identifier("foo")));
		ASTObject expr16 = UnopExpr.build("++", new VarExpr(new Identifier("foo")));
		ASTObject expr17 = UnopExpr.build("--", new VarExpr(new Identifier("foo")));
		ASTObject expr18 = new NumericOpExpr(new NumericOpExpr(new NumericOpExpr(new IntLiteralExpr(1), "*", new IntLiteralExpr(3)), "/", new IntLiteralExpr(2)), "*", new IntLiteralExpr(2));
		ASTObject expr19 = new NumericOpExpr(new NumericOpExpr(new IntLiteralExpr(1), "*", new IntLiteralExpr(3)), "-", new NumericOpExpr(new IntLiteralExpr(4), "/", new IntLiteralExpr(2)));
		ASTObject expr20 = new NumericOpExpr(new IntLiteralExpr(1), "-", new IntLiteralExpr(2));
		ASTObject expr21 = new ShiftOpExpr(new IntLiteralExpr(1), ">>", new VarExpr(new Identifier("foo")));
		ASTObject expr22 = new NumericTestOpExpr(new IntLiteralExpr(1), ">", new IntLiteralExpr(1));
		ASTObject expr23 = new NumericTestOpExpr(new VarExpr(new Identifier("foo")), "==", new IntLiteralExpr(2));
		ASTObject expr24 = new InstanceofExpr(new VarExpr(new Identifier("foo")), new ResolvedTypeD(new MajUncheckedType(new Identifier("bar"))));
		ASTObject expr25 = new BitwiseOpExpr(new VarExpr(new Identifier("foo")), "&", new VarExpr(new Identifier("bar")));
		ASTObject expr26 = new BitwiseOpExpr(new BitwiseOpExpr(new VarExpr(new Identifier("foo")), "&", new VarExpr(new Identifier("bar"))), "|", new VarExpr(new Identifier("foobar")));
		ASTObject expr27 = new NumericTestOpExpr(new IntLiteralExpr(2), "&&", new IntLiteralExpr(3));
		ASTObject cond1 = new NumericTestOpExpr(new VarExpr(new Identifier("x")), "==", new VarExpr(new Identifier("y")));
		ASTObject selfexp1 = IDENT;
		ASTObject selfexp2 = IDENT;
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
		System.out.println(expr21.unparse());
		System.out.println(expr22.unparse());
		System.out.println(expr23.unparse());
		System.out.println(expr24.unparse());
		System.out.println(expr25.unparse());
		System.out.println(expr26.unparse());
		System.out.println(expr27.unparse());
		System.out.println(cond1.unparse());
		System.out.println(selfexp1.unparse());
		System.out.println(selfexp2.unparse());
	}
}
