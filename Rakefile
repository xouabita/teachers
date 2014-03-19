require 'fileutils'

def del arr
  if arr.is_a? String then arr = [arr] end
  arr.each do |str|
    if File.exist?(str) then
      if File.directory?(str) then FileUtils.rm_rf str
      else FileUtils.rm str end
    end
  end
end

task :default => 'build'

desc "Build the proj"
task :build do
  sh "g++ main.cpp -pedantic -Wall -o main.out"
end

desc "Make the test"
task :test => :build do
  del 'results'
  FileUtils.mkdir 'results'

  in_files = Dir.glob('tests/in_?.txt')
  out_files = Dir.glob('tests/out_?.txt')

  in_files.each do |inp|
    out = inp.sub('tests/','results/').sub('in','out')
    sh "./main.out #{inp} #{out}"
  end

  puts "--------------------"

  res_files = Dir.glob('results/out_?.text')
  res_files.each do |res|
    out_files.each do |o|
      if o.sub('tests/', "") == res.sub('results/', "") then
        if (sh "diff #{o} #{res}") then puts 'OK!'
        else puts 'KO!' end
      end
    end
  end

end

desc "Clean the dir"
task :clean do
  del 'results'
  del 'main.out'
end
