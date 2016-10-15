require 'tempfile'
require 'fileutils'
require 'daru'

cities = {
  sao_paulo: [3_304_557, 'São Paulo'],
  rio_de_janeiro: [3_550_308, 'Rio de Janeiro']
}

days = %w(Segunda Terça Quarta Quinta Sexta Sábado Domingo)

df = Daru::DataFrame.from_csv '../out.txt'

cities.each do |name, info|
  data = df.filter_rows { |r| r[2] == info[0] }
  means = []
  std = []
  cdata = ""

  days.each_index do |i|
    means[i] = data.filter_rows { |r| r[1] == i } [3].mean
    std[i] = data.filter_rows { |r| r[1] == i } [3].std
    cdata += '"' + days[i] + '",' + means[i].to_s + ", " + std[i].to_s + "\n"
  end

  data_file = Tempfile.new ['lightning_data', 'csv'], Dir.pwd
  data_file.write cdata
  data_file.flush

  script = <<EOF
  set terminal pngcairo font "Helvetica"
  set output "#{Dir.pwd}/#{name}.png"
  set title "#{info[1].to_s}"
  set xlabel "Dia da semana"
  set ylabel "Número médio de raios"
  set grid
  set datafile sep ','
  set style histogram errorbars gap 1 lw 1
  set style data histograms
  set style fill solid 0.3  
  plot '#{data_file.path}' using 2:3:xtic(1) title "Dados"
  unset output
EOF

  script_file = Tempfile.new "lightning_plot", Dir.pwd
  script_file.write script
  script_file.flush

  puts "Saving #{name}"
  if not system("gnuplot", script_file.path)
    puts "And error ocurred."
  end

  script_file.unlink
  data_file.unlink
end
