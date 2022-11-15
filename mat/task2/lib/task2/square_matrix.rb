# frozen_string_literal: true
# typed: strict

require "sorbet-runtime"

module Task2
	class SquareMatrix
		extend T::Sig

		sig { returns(Integer) }
		attr_reader :size

		sig { params(size: Integer, matrix: T.nilable(T::Array[T::Array[Float]])).void }
		def initialize(size, matrix: nil)
			@size = size
			@matrix = T.let(Array.new(size*size, 0.0), T::Array[Float])

			return if matrix.nil?

			size.times do |i|
				if matrix[i].nil?
					@matrix.fill(0.0, i * size, size)
				else
					@matrix[i * size, size] = T.must(matrix[i])
				end
			end
		end

		sig { params(i: Integer, j: Integer).returns(T.nilable(Float)) }
		def [](i, j)
			@matrix[i * @size + j]
		end

		sig { params(i: Integer, j: Integer, value: Float).returns(Float) }
		def []=(i, j, value)
			@matrix[i * @size + j] = value
		end

		sig { params(index: T.nilable(Integer), value: T.any(Float, T::Array[Float])).void }
		def fill(index: nil, value: 0.0)
			if index.nil?
				@matrix.fill(T.cast(value, Float))
			else
				@matrix[index * @size, @size] = T.cast(value, T::Array[Float])
			end
		end

		sig { returns(T::Array[Task2::Edge]) }
		def edges
			edges = T.let(Array[], T::Array[Task2::Edge])

			@size.times do |u|
				(u + 1).upto(@size - 1) do |v|
					dist = T.must(self[u, v])
					if dist != 0
						edges.append(Edge.new(u, v, dist))
					end
				end
			end

			edges
		end
	end
end
