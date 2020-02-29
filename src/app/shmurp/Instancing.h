#pragma once

#include "commons.h"

#include <renderer/VertexSpecification.h>


namespace ad {


// TODO This trait is more general, should be moved in Engine or Render
/// \brief Trait to get from a video card buffer data type to its description
template <class T_bufferData>
class BufferData_trait;


template <class T_bufferData>
AttributeDescriptionList BufferData_trait_description =
    BufferData_trait<T_bufferData>::description_value;


template <class T_vertex, class T_instance>
class Instancing
{
public:
    Instancing(
            gsl::span<const T_vertex> aVertexData,
            gsl::span<const T_instance> aInstanceData,
            GLenum aMode,
            Vec<4, GLfloat> aColor);

    template <class T_iterator, class T_unaryOperation>
    void updateIBO(T_iterator aBegin, const T_iterator aEnd, T_unaryOperation && aOperation);

    void updateIBO(gsl::span<const T_instance> aInstanceData);

    void draw() const;

private:
    VertexArrayObject mVAO;
    VertexBufferObject mVertexBO;
    VertexBufferObject mInstanceBO;
    GLenum mMode;

    GLsizei mVertexCount{0};
    GLsizei mInstanceCount{0};

    // TODO Handle uniforms in a generic manner
public: // TODO privatize
    Vec<4, GLfloat> mColor{Vec<4, GLfloat>::Zero()};

};

template <class T_vertex, class T_instance>
Instancing<T_vertex, T_instance>::Instancing(
        gsl::span<const T_vertex> aVertexData,
        gsl::span<const T_instance> aInstanceData,
        GLenum aMode,
        Vec<4, GLfloat> aColor) :
    mVAO(),
    mVertexBO(loadVertexBuffer(mVAO, BufferData_trait_description<T_vertex>, aVertexData)),
    mInstanceBO(loadVertexBuffer(mVAO, BufferData_trait_description<T_instance>, aInstanceData, 1)),
    mMode{aMode},
    mVertexCount(aVertexData.size()),
    mColor(aColor)
{}

template <class T_vertex, class T_instance>
template <class T_iterator, class T_unaryOperation>
void Instancing<T_vertex, T_instance>::updateIBO(
        T_iterator aBegin,
        const T_iterator aEnd,
        T_unaryOperation && aOperation)
{
    std::vector<T_instance> buffer;
    buffer.reserve(std::distance(aBegin, aEnd));

    std::transform(aBegin, aEnd, std::back_inserter(buffer),
                   std::forward<T_unaryOperation>(aOperation));

    respecifyBuffer(mInstanceBO, buffer);
    mInstanceCount = buffer.size();
}

template <class T_vertex, class T_instance>
void Instancing<T_vertex, T_instance>::updateIBO(gsl::span<const T_instance> aInstanceData)
{
    respecifyBuffer(mInstanceBO, aInstanceData);
    mInstanceCount = aInstanceData.size();
}

template <class T_vertex, class T_instance>
void Instancing<T_vertex, T_instance>::draw() const
{
    glBindVertexArray(mVAO);
    glDrawArraysInstanced(mMode,
                          0,
                          mVertexCount,
                          mInstanceCount);
}

} // namespace ad
