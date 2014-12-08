package maj.ajTest;

import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class stmttest {

    public static void main(String myargs[]) {
	// Statement
	infer var1 =  `[int x;];
	infer expr1 = `(Expr)[foo];

	infer stat1 =  `[x++;];
	infer stat1A =  `[x=0;];
	infer stat2 =  `[do {} while(true);];

	infer stat3 =
		`[for (int x=0; x!=5; x++) {
			y=(Foo)Bar(x);
		}
		];

	infer stat3A =
		`[for (#(VarDec)var1 x!=5; x++) {
			Bar(x);
		}
		];

	infer stat4 =
		`[if (x==y)
			x++;
		else
			y++;
		];

	infer stat4A =
		`[if (#(Expr) expr1)
			x++;
		else
			y++;
		];
	infer stat4A1 =
		`[if (#expr1)
			x++;
		else
			y++;
		];
	infer stat4B =
		`[if (x==y)
			#(Statement) stat1
		else
			y++;
		];

	infer stat4C =
		`[if (x==y)
			x++;
		else
			#(Statement) stat2
		];

	infer stat4D =
		`[if (x==y)
			x++;
		else {
			#(VarDec) var1
		}
		];

	infer statE =
		`[if (x==y)
			x++;
		else {
			#(ClassDec)class11
		}
		];

//	infer selfstat1 = `(Statement)[#stat1];	//??? Why doesn't this work?
	infer selfstat2 = `(Statement)[#(Statement)stat1];


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
