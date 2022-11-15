# frozen_string_literal: true
# typed: strict

require "sorbet-runtime"

module Task2
	extend T::Sig

	sig { params(matrix: SquareMatrix).returns([T::Array[Task2::Edge], Float, T::Boolean]) }
	def self.Kruskal(matrix)
		edges = matrix.edges.sort
		set = DisjointSet.new(size: matrix.size)
		output = T.let(Array[], T::Array[Task2::Edge])
		cost = 0.0

		edges.each do |e|
			if set[e.u] != set[e.v]
				cost += e.dist
				output.append(e)
				set.unite(e.u, e.v)
			end
		end

		return output, cost, set.coherent?
	end
end
