package main

import (
	"net/http"
)

func main() {
	url, err := url.Parse("http://172.17.0.2")
	if err != nil {
		log.Fatal(err)
	}

	proxy := http.HandlerFunc(func(rw http.ResponseWriter, r *http.Request) {
		req.Host = url.Host
		req.RequestURI = ""
		req.URL.Host = url.Host
		req.URL.Scheme = url.Scheme

		s, _, _ := net.SplitHostPort(req.RemoteAddr)
		req.Header.Set("X-Forwarded-For", s)

		resp, err := http.DefaultClient.Do(req)
		if err != nil {
			rw.WriteHeader(http.StatusInternalServerError)
			fmt.Fprint(rw, err)
			return
		}

		for key, values := range resp.Header {
			rw.Header().Del(key)
			for _, value := range values {
				rw.Header().Add(key, value)
			}
		}

		done := make(chan bool)

		go func() {
			for {
				select {
				case <-time.Tick(10*time.Millisecond):
					rw.(http.Flusher).Flush()
				case <-done:
					return
				}
			}
		}()

		rw.WriteHeader(resp.StatusCode)
		io.Copy(rw, resp.Body)
		close(done)
	})

	http.ListenAndServe(":8080", proxy)
}
