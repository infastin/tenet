# frozen_string_literal: true
# typed: strict

require "sorbet-runtime"

module Task2
	class Edge
		extend T::Sig

		sig { returns(Integer) }
		attr_reader :u, :v

		sig { returns(Float) }
		attr_reader :dist

		sig { params(u: Integer, v: Integer, dist: Float).void }
		def initialize(u, v, dist)
			@u = u
			@v = v
			@dist = dist
		end

		sig { params(rhs: Edge).returns(Integer) }
		def <=>(rhs)
			@dist <=> rhs.dist
		end
	end
end
