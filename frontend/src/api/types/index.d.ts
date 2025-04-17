/// @{ common
declare interface Data<T> {
  data: T
}

declare interface PaginateResponse<T> {
  list: T[]
  total: number
  page: number
  pageSize: number
  maxPage: number
}

declare interface IdResponse {
  id: number
}

declare interface MessageResponse {
  message: string
}

declare interface ErrorResponse {
  error: string
  code: number
}
/// @}

/// @{ auth
declare interface LoginParams {
  username: string
  password: string
}
/// @}

/// @{ user
declare interface UserQuery {
  username?: string
  phoneNumber?: string
  status?: number
  dateRange?: Date[] | string
}

declare interface PageQuery {
  page: number
  pageSize: number
}

declare interface User {
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

declare interface newUser {
  nickname: string
  phone: string
  username: string
}

declare interface UserUpdate {
  user_id: number
  nickname: string
  phone: string
}

declare interface BatchDeleteUserParams {
  ids: number[],
  password: string
}
/// @}