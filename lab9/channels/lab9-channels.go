package main


import(
	"fmt"
)

func routine(c chan string){

	fmt.Println(<- c)

	c <- "Oi Main, bom dia, tudo bem?"

	fmt.Println(<- c)

	c <- "Certo, entendido."
	
	<-c
	fmt.Println("[Goroutine] finalizando ... ")
	c <- " "
}

func main(){

	c := make(chan string)

	go routine(c)

	c <- "Olá, Goroutine, bom dia!"

	fmt.Println(<-c)

	c <- "Tudo bem! Vou terminar, tá?"

	fmt.Println(<-c)

	c <- " " //Garantir que a thread finalize antes da main
	<- c
	fmt.Println("[Main] finalizando ... ")


}