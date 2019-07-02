#!/usr/local/bin/guash

println("Content-type: text/html\n")

println("<html>")

println("<head>")
println("<title>Environment</title>")
println("</head>")

println("<body>")

println("argc = " + argc)
println("<br>")
println("<br>")

for (j = 0; j < argc; j = j + 1) {
    println("argv[" + j + "] = " + argv[j])
    println("<br>")
}

println("<br>")

foreach (env; name; value) {
    println("env[\"" + name + "\"] = " + "\"" + value + "\"")
    println("<br>")
}

println("</body>")

println("</html>")
