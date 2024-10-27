package main

import(
	"fmt"
	"math/rand"
	"math"
	"time"
)


func ehPrimo(n int) bool{
	if n <= 1  || n % 2 == 0{
		return false
	}
	if n == 2 {
		return true
	}

	for i := 3; float64(i) < math.Sqrt(float64(n)) + 1; i++{
		if n % i == 0{
			return false
		}
	}
	return true

}




func main(){
	//Número de threads
	N:= 1000
	M := 8
	
	//Criação da comunicação
	prime_count_channel := make(chan int, M)
	nums := make(chan int, N)

	//Criação das threads
	for i:=0; i< M ; i++{

		go func(num chan int,  c chan int){
			count := 0
			for{
				n, ok :=  <- nums

				if(!ok){
					break
					} 

				if ehPrimo(n){
					//fmt.Println(n, "é primo") //Log 
					count += 1
				}

			}

			c <- count

		}(nums, prime_count_channel)

	}


	//Geração dos valores
	rand.New(rand.NewSource(int64(time.Now().Nanosecond())))
	for i := 0 ; i< N; i++ {
		nums <- rand.Intn(1000)
	}
	//Todos os valores foram gerados
	close(nums)
	
	//União dos resultados
	prime_count := 0
	for i:=0; i< M; i++{
		prime_count += <-prime_count_channel
	}

	fmt.Println("Existem ", prime_count, " primos nessa lista")
}