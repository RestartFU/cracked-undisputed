package main

import (
	"net/http"
	"testing"
)

func TestResponse(t *testing.T) {
	get, err := http.Get("http://localhost:1337")
	if err != nil {
		panic(err)
	}
	var s string
	for h, v := range get.Header {
		s += h + ":" + " " + v[0] + "\n"
	}
	panic(get.StatusCode)
}
