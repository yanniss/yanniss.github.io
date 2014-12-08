public class metaAJTest {
    public static void main(String[] myArgs) {

	// Identifier
	infer id1 = `[foo];
	Identifier id2 = `[foo];
	infer id3 = `(Identifier)[foo];
	Identifier id4 = `(Identifier)[foo];
	infer pack1 = `[foo.bar];
	infer pack2 = `[java.lang.#pack1];

	infer selfid1 = `(Identifier)[#pack1];
	infer selfid2 = `(Identifier)[#(Identifier)pack1];
	infer selfid3 = `[#(Identifier)pack1];
	infer crazyid = `[#(Identifier)[x+y]];

	// Name Pattern
	infer name1 = `[*];
	NamePattern name2 = `[Foo.*];
	infer name3 = `(NamePattern)[Foo.*];
	NamePattern name4 = `(NamePattern)[foo];
	infer packStar = `[java.lang.*];

	infer selfname1 = `(NamePattern)[#pack1];
//	infer selfname2 = `(NamePattern)[#(NamePattern)pack1];

	// Modifiers
	infer mods1 = `[public static];
	Modifier[] mods2 = `[public static];
	infer mods3 = `(Modifier[])[public static];
	Modifier[] mods4 = `(Modifier[])[public static];
	infer odd = `[];

	infer selfmods1 = `(Modifier[])[#mods4];
	infer selfmods2 = `(Modifier[])[#(Modifier[])mods4];

	// Import
	infer imp1 = `[import java.lang.*;];
	Import imp2 = `[import java.lang.*;];
	infer imp3 = `(Import)[import java.lang.*;];
	Import imp4 = `(Import)[import java.lang.*;];

	infer selfimp1 = `(Import)[#imp4];
	infer selfimp2 = `(Import)[#(Import)imp4];

	// Type Spec
	infer type1 = `[int];
	infer type2 = `[int[]];
	TypeDec type3 = `[int];
	infer type4 = `(TypeD)[int];
	TypeDec type5 = `(TypeD)[int[]];
	TypeDec type6 = `(TypeD)[Foo.Bar];
	TypeDec type7 = `(TypeD)[#type1];
	TypeDec type8 = `(TypeD)[#(Identifier)type1];

	infer selftyp1 = `(TypeD)[#type4];
	infer selftyp2 = `(TypeD)[#(TypeD)type4];

	// Var. Dec.
	infer var1 =  `[int x;];
	infer var1 =  `[int x,y;];
	infer var2 =  `[int[] x;];
	infer var3 =  `[int x[];];
	infer var4 =  `[Foo x;];
	infer var5 =  `[Foo[] x;];
	infer var6 =  `[Foo x[];];
	infer var7 =  `[Foo.Bar x;];
	infer var8 =  `[Foo.Bar[] x;];
	infer var9 =  `[Foo.Bar x[];];

	infer var10 =  `[int x=3;];
	infer var11 =  `[int x[]={1, 2, 3};];

	// (intertype declarations)
	infer var12 =  `[Foo Bar.x;];
	infer var13 =  `[Foo x,Bar.y;];

	infer var1A =  `(VarDec)[int x;];
	infer var1B =  `[int #id1;];
	infer var1C =  `[int #(Identifier)id1;];

	infer var2A =  `(VarDec)[int[] x;];
	infer var2B =  `[int[] #id1;];
	infer var2C =  `[int[] #(Identifier)id1;];

	infer var3A =  `(VarDec)[int x[];];
	infer var3B =  `[int #id1[];];
	infer var3C =  `[int #(Identifier)id1[];];

	infer var4A =  `(VarDec)[Foo x;];
	infer var4B =  `[Foo #id1;];
	infer var4C =  `[Foo #(Identifier)id1;];

	infer var5A =  `(VarDec)[Foo[] x;];
	infer var5B =  `[Foo[] #id1;];
	infer var5C =  `[Foo[] #(Identifier)id1;];

	infer var6A =  `(VarDec)[Foo x[];];
	infer var6B =  `[Foo #id1[];];
	infer var6C =  `[Foo #(Identifier)id1[];];

	infer var7A =  `(VarDec)[Foo.Bar x;];
	infer var7B =  `[Foo.Bar #id1;];
	infer var7C =  `[Foo.Bar #(Identifier)id1;];

	infer var8A =  `(VarDec)[Foo.Bar[] x;];
	infer var8B =  `[Foo.Bar[] #id1;];
	infer var8C =  `[Foo.Bar[] #(Identifier)id1;];

	infer var9A =  `(VarDec)[Foo.Bar x[];];
	infer var9B =  `[Foo.Bar #id1[];];
	infer var9C =  `[Foo.Bar #(Identifier)id1[];];

//	infer varE =  `[#(TypeD)type1 #(Identifier)id1;];

//	infer selfvar1 = `(VarDec)[#var1];
//	infer selfvar2 = `(VarDec)[#(VarDec)var1]; VarDec does not work???

	// Expression
	infer cond1 =  `[x==y];
	infer cond2 =  `(Expr)[x==y];
	infer selfexp1 = `(Expr)[#cond1];
	infer selfexp2 = `(Expr)[#(Expr)cond1];

	// Statement
	infer stat1 =  `[x++;];
	infer stat2 =  `[x=0;];
	infer stat2 =  `[do {}while(true);];

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
		`[if (#(Expr) exp1)
			x++;
		else
			y++;
		];
	infer stat4A1 =
		`[if (#exp1)
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

	// Method.
	infer meth1 =
		`[int foo(int x, int y) {
			#(Modifier[])mods1 int z;
			x++;
			return x;
		}
		];

	infer cons1 =
		`[foo(int x, int y) {
			x++;
			return x;
		}
		];

	infer meth1A =
		`[int foo(int x, int y) {
			int z;
			x++;
			return x;
		}
		];
	infer meth1B =
		`[int foo(int x, int y) {
			#(VarDec)varA
			x++;
			return x;
		}];

/* How do we fit Identifier in here???
	infer meth1C =
		`[int foo (int #(Identifier)id1, #(Identifier)id2) {
			int z;
			x++;
			return x;
		}];
*/
	infer meth1D =
		`[int foo(int x, int y) {
			int z;
			#(Statement) stat1;
			return x;
		}];

	infer meth2 = `[Foo.Bar baz(int x, int y);];


	// Class Declaration
	infer class0 =
		`[class foo {}];

	infer class1 =
		`[class foo {
			public int x;

			int foo(int x, int y) {
				int z;
				x++;
				return x;
			}
		} ];

	infer class1A =
		`[class foo {
			#(Modifier[])mods1 int x;

			#(Modifier[])mods2 foo(int x, int y) {
				int z;
				x++;
				return x;
			}
		} ];

/* How do we fit Identifier in here???
	infer class1B =
		`[class #(Identifier)id1 {
			public int x;

			int foo(int x, int y) {
				int z;
				x++;
				return x;
			}
		} ];
*/
	infer class1C =
		`[class foo {
			#(VarDec)var1

			int foo(int x, int y) {
				int z;
				x++;
				return x;
			}
		} ];

	infer class1D =
		`[class foo {
			public int x;

			#(MethodDec)meth1

		} ];
//	infer class2 =  `(ClassDec)[class foo { int x; #var1 } ];	// Ambiguous

//	infer selfclass1 =  `(ClassDec)[#(ClassDec)class1];
//	infer selfclass1 =  `(ClassDec)[#class1];	//Doesn't work yet b'cs ambiguous with Statement


	// Interface Declaration
	infer inter1 =
		`[interface foo {
			public int x;

			int foo(int x, int y) {
				int z;
				x++;
				return x;
			}
		} ];


	// Compilation Unit
	infer cu1 =
		`[package blah.baz;

			class foo {}
		];

	infer cu2 =
		`[import java.lang.*;

			class foo {}
		];

	infer cu3 =	//Hmmm, where is foo2???
		`[
		class foo1 {}
		class foo2 {}
		];


	// ASPECTJ-SPECIFIC CONSTRUCTS

	// Pointcut
	infer pct1 =
		`[call (void Foo.m(int, int))];
	infer pct2 =
		`[call (Foo.new(int))];
	infer pct3 =
		`[execution (void Foo.m(int))];
	infer pct4 =
		`[execution (Foo.new(int))];
	infer pct5 =
		`[initialization (Foo.new(int))];
	infer pct6 =
		`[preinitialization (Foo.new(int))];
	infer pct7 =
		`[staticinitialization (Foo.new(int))];
	infer pct8 =
		`[get (Foo x)];
	infer pct9 =
		`[set (Foo x)];
	infer pct10 =
		`[handler (IOException)];
	infer pct11 =
		`[adviceexecution ()];
	infer pct12 =
		`[within (Foo.Bar.*)];
	infer pct13 =
		`[withincode (void Foo.m(int))];
	infer pct14 =
		`[withincode (Foo.new(int))];
	infer pct15 =
		`[withincode (!private void Foo.*(int))];

	infer pct20 =
		`[cflow (call (void Foo.m(int)))];
	infer pct21 =
		`[cflowbelow (call (void Foo.m(int)))];
	infer pct24 =
		`[target (java.io.InputPort)];
	infer pct25 =
		`[args (java.io.InputPort, int)];
	infer pct26 =
		`[args (int, *)];
	infer pct27 =
		`[args (int, .., int)];

	infer pct28 =
		`[this (Point)];
	infer pct28 =
		`[if (x==y)];

	infer pct29 =
		`[!args (int, .., int)];
	infer pct30 =
		`[args (int, .., int) && withincode (Foo.new(int))];
	infer pct31 =
		`[args (int, .., int) || withincode (Foo.new(int))];

	infer pct35 =
		`(Pointcut)[if (x==y)];
/*
	// Advice
	infer adv1 =
		`[before():get(int Foo.y) {}];
	infer adv2 =
		`[before(int i):get(int Foo.y) {}];
	infer adv3 =
		`[before(Object o):get(int Foo.y) {}];
	infer adv4 =
		`[after():get(int Foo.y) {}];
	infer adv5 =
		`[after(int i):get(int Foo.y) {}];
	infer adv6 =
		`[after(Object o):get(int Foo.y) {}];
	infer adv7 =
		`[after() throwing:get(int Foo.y) {}];
	infer adv8 =
		`[after() throwing(NotFoundException e):get(int Foo.y) {}];
	infer adv9 =
		`[Object around(): call(int Foo.m(int)) {}];

	// Pointcut Declaration
	infer pdec1 =
		`[private pointcut pc();];
	infer pdec2 =
		`[pointcut pc(int i):set(int Foo.x) && args(i);];
	infer pdec4 =
		`[abstract pointcut pc(Object o);];
	infer pdec3 =
		`[abstract pointcut pc();];

	// Inter-type Declarations
	infer idec1 =
		`[declare parents: Foo extends Bar;];
	infer idec2 =
		`[declare parents: Foo implements Bar1,Bar2;];
	infer idec3 =
		`[declare warning: set(* Point.*) && !within(Point):"bad set";];
	infer idec4 =
		`[declare error: call(Singleton.new(..)):"bad construction";];
	infer idec5 =
		`[declare soft: IOException: execution(Foo.new(..));];
	infer idec6 =
		`[declare precedence: Security, Logging;];

	// Aspect Definition
	infer aspect1 =
		`[aspect Foo {
			declare precedence: Security, Logging;

			before():get(int Foo.y) {}

			abstract pointcut pc(Object o);

			Object around(): call(int Foo.m(int)) {}

			int around(): call(int Foo.m(int)) {}

			public int x;

			int foo(int x, int y) {
				int z;
				x++;
				return x;
			}
		} ];
*/

	infer aspect2 =
		`[aspect Foo {
			#(DecPattern) pdec1
			#(Advice) adv1
			#(PointcutDec) pdec1
			#(VarDec)var1
			#(MethodDec)meth1
		} ];

    }
}

