#!/usr/bin/ruby

require 'thread'
require 'socket'
require 'timeout'

class ResultEntry
	attr_reader :time
	def initialize time
		@time = time
	end
end

$server = {address: "dev.voltapps.ru", port: 90}
$threads = 100
$requests = 10
$requests_total = $threads*$requests
$receive_text = '<?xml version="1.0"?>
<cross-domain-policy>
  <allow-access-from domain="*" to-ports="*"/>
</cross-domain-policy>'

$result_array = []
$threads_array = []
$semaphore = Mutex.new

def add_result data
	$semaphore.synchronize {
		$result_array.concat data
	}
end

puts "Start #{$requests_total} requests to server"
puts "..."

$threads.times {

	$threads_array << Thread.new {
		result_t = []

		$requests.times {
			begin
				beginning_time = Time.now
				s = TCPSocket.new $server[:address], $server[:port]
				a = s.recv $receive_text.bytesize
				end_time = Time.now

				raise "Incorrect response text #{a.to_s}" if a.to_s != $receive_text

				result_t << ResultEntry.new((end_time - beginning_time)*1000)
				
				s.close
			rescue Exception => e  
				puts e.message  
				puts e.backtrace.inspect 
			end
		}

		add_result result_t
	}
}

$threads_array.each {|thread| thread.join }

puts "Finish"
puts "Results: #{$requests_total} requests; #{($result_array.size.to_f / $requests_total * 100).floor}% success;"

time_array = $result_array.map {|item| item.time}
puts "Minimal #{time_array.min} milliseconds; Maximum #{time_array.max} milliseconds; Average #{time_array.size == 0 ? 0 : time_array.reduce(0, :+)/time_array.size } milliseconds;"

