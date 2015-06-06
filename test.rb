#!/usr/bin/ruby

require 'thread'

class ResultEntry
	attr_reader :time
	def initialize time
		@time = time
	end
end

server = {address: "dev.voltapps.ru", ports: [90, 843]}
threads = 100
requests = 100
requests_total = threads*requests

result_array = []
semaphore = Mutex.new

def add_result data
	semaphore.synchronize {
		result_array.concat data
	}
end

puts "Start #{requests_total} requests to server"
puts "..."

threads.times {
	Thread.new {
		result_t = []

		requests.times {
			begin
				beginning_time = Time.now
				s = TCPSocket.new server[:address], server[:port]

				a = s.recv 6

				end_time = Time.now

				result_t << ResultEntry.new((end_time - beginning_time)*1000)
				
				s.close
			rescue
				puts "shit happens"
			end
		}

		add_result result_t
	}
}

puts "Finish"
puts "Results: #{requests_total} requests; #{(result_array.size.to_f / requests_total * 100).floor} success;"

time_array = result_array.map {|item| item.time}
puts "Minimal #{time_array.min} milliseconds; Maximum #{time_array.max} milliseconds; Average #{time_array.size == 0 ? 0 : time_array.reduce(0, :+)/time_array.size } milliseconds;"

