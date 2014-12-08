dir /b /s *.java > jorch_source.txt
javac @jorch_source.txt
rmic -v1.2 jorchestra.runtime.ObjectFactory.ObjectFactoryImpl
del jorch_source.txt
