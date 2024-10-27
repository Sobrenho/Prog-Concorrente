package main

import(
	"fmt"
	"math/rand"
	"math"
	"time"
)


func ehPrimo(n int) bool{
	if n <= 1 {
		return false
	}
	if n == 2 {
		return true
	}
	if n % 2 ==0{
		return false
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
	prime_count := make(chan int, M)
	nums := make(chan int, N)

	//Criação das threads
	for i:=0; i< M ; i++{

		go func(){
			count := 0
			for{
				n, ok :=  <- nums

				if(!ok){
					break
					} 
				if ehPrimo(n){
					count += 1
				}
			}
			prime_count <- count

		}()

	}


	//Geração dos valores
	rand.New(rand.NewSource(int64(time.Now().Nanosecond())))
	for i := 0 ; i< N; i++ {
		nums <- rand.Intn(1000)
	}
	//Todos os valores foram gerados
	close(nums)
	
	//União dos resultados
	num_primes := 0
	for i:=0; i< M; i++{
		num_primes += <-prime_count
	}

	fmt.Println("Existem ", num_primes, " primos nessa lista")
}