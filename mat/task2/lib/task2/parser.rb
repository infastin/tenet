# typed: strict
# frozen_string_literal: true

require "sorbet-runtime"
require "optparse"

module Task2
	class Options < T::Struct
		prop :filename, T.nilable(String), default: nil
	end

	class Parser
		extend T::Sig

		sig { params(options: T::Array[String]).returns(Options) }
		def self.parse(options)
			args = Options.new

			option_parser = OptionParser.new do |parser|
				parser.banner = "Usage: task2 [options]"

				parser.on("-fFILE", "--file=FILE", "Filename") do |f|
					args.filename = f
				end

				parser.on("-h", "--help", "Print help message") do
					puts parser
					exit
				end
			end

			option_parser.parse!(options)
			args
		end
	end
end
