package main


import(
	"fmt"
)

func routine(c chan string){
	var msg string

	msg = <- c
	fmt.Println(msg)

	c <- "Oi Main, bom dia, tudo bem?"

	msg = <- c
	fmt.Println(msg)

	c <- "Certo, entendido."
	
	msg = <-c
	fmt.Println("finalizando ...")
	c <- msg 
}

func main(){
	var msg string

	c := make(chan string)

	go routine(c)

	c <- "Olá, Goroutine, bom dia!"

	msg = <-c
	fmt.Println(msg)

	c <- "Tudo bem! Vou terminar, tá?"

	msg = <-c
	fmt.Println(msg)

	c <- " " //Garantir que a thread finalize antes da main

	fmt.Println("finalizando ...")

	<- c

}