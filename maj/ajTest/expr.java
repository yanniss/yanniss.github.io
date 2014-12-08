package maj.ajTest;

import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class exprtest {

    public static void main(String myargs[]) {
	// Expression
	int[] var = {1, 2, 3};

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
	infer expr20 = `(Expr) [1 -2 ];
	infer expr21 = `(Expr)[1 >> foo];
	infer expr22 = `(Expr)[1 > 1];
	infer expr23 = `(Expr)[foo == 2];
	infer expr24 = `(Expr)[foo instanceof bar];
	infer expr25 = `(Expr)[foo & bar];
	infer expr26 = `(Expr)[foo & bar | foobar];
	infer expr27 = `(Expr)[2 && 3];
	/* STACK OVERFLOW ERROR ON THIS. UNRESOLVED */
	//	infer expr28 = `(Expr)[2 && 3 ? foo : bar];

	infer expr28 = `(Expr)[foo += 1];

	infer cond1 =  `[x==y];
	infer selfexp1 = `(Expr)[#cond1];
	infer selfexp2 = `(Expr)[#(Expr)cond1];

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
	System.out.println(expr28.unparse());
	System.out.println(cond1.unparse());
	System.out.println(selfexp1.unparse());
	System.out.println(selfexp2.unparse());

    }
}
