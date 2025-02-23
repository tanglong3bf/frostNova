import request from "@/utils/request";

export interface UserQuery {
    username?: string
    phoneNumber?: string
    status?: number
    dateRange: Date[] | string
}

export interface pageQuery {
    page: number
    pageSize: number
}

export interface User {
    user_id: number
    username: string
    password?: string
    nickname: string
    dept: {
        dept_name: string
    }
    phone: string
    status: number
    create_time: string
    selected: boolean
}

export interface newUser {
    nickname: string
    phone: string
    username: string
}

export interface PaginateResponse<T> {
    list: T[]
    total: number
    page: number
    pageSize: number
    maxPage: number
}

export const getUserList = (query: UserQuery & pageQuery): Promise<PaginateResponse<User>> => {
    console.log(query)
    if (Array.isArray(query.dateRange)) {
        query.dateRange = query.dateRange.join(',')
    }
    return request.get<PaginateResponse<User>, PaginateResponse<User>, void>(
        '/user',
        {
            params: query
        })
}

export const updateStatus = (userId: number, status: number): Promise<void> => {
    return request.put<void, void, void>(`/user/${userId}/status/${status}`)
}

export const newUser = (user: newUser): Promise<{ id: number }> => {
    return request.post<{ id: number }, { id: number }, newUser>('/user', user)
}

export const deleteUser = (userId: number): Promise<void> => {
    return request.delete<void, void, void>(`/user/${userId}`)
}