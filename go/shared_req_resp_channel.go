package main

// http://dieter.plaetinck.be/post/beautiful_go_patterns_for_concurrent_access_to_shared_resources_and_coordinating_responses/
// demo the fact that we can just use one shared req and one resp channel.
// as long as they are unbuffered, the synchronization works just fine.

import "fmt"
import "sync"
import "math/rand"
import "time"

var requests = make(chan int)
var responses = make(chan string)

func routine(num int, wg *sync.WaitGroup) {
	defer wg.Done()
	// pretend this is a routine that's doing something, like serving a user session
	// but then we need to modify some shared state
	time.Sleep(time.Duration(rand.Intn(100)) * time.Millisecond)
	requests <- num
	time.Sleep(time.Duration(rand.Intn(100)) * time.Millisecond) // simulate some heavy lifting
	resp := <-responses
	fmt.Printf("routine %d gets response: %s\n", num, resp)
}

func coordinator() {
	for {
		req := <-requests
		// in here, you can do whatever modifications to shared state you need.
		time.Sleep(time.Duration(rand.Intn(100)) * time.Millisecond) // simulate some heavy lifting
		responses <- fmt.Sprintf("this return value is meant for routine %d", req)
	}
}

func main() {
	go coordinator()

	var wg sync.WaitGroup
	for i := 0; i < 100; i++ {
		wg.Add(1)
		go routine(i, &wg)
	}
	wg.Wait()
	close(requests)
}
